// 키스토어 비밀번호를 확인하고, 맞으면 gradle.properties 에 Gradle이 읽을 수 있는
// 형태(유니코드 이스케이프)로 적어준다.
//
// 실행:  "C:\Program Files\Android\Android Studio\jbr\bin\java.exe" KeystorePwFix.java
//
// - 비밀번호는 GUI 입력창으로 받는다(콘솔 코드페이지 때문에 한글이 깨지는 것을 피하려고).
// - 비밀번호를 화면이나 로그에 출력하지 않는다.
// - gradle.properties 를 고치기 전에 .bak 백업을 남긴다.

import javax.swing.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.security.KeyStore;
import java.util.*;

public class KeystorePwFix {

    static final String PROPS = System.getProperty("user.home") + "\\.gradle\\gradle.properties";

    static char[] ask(String title) {
        JPasswordField pf = new JPasswordField(24);
        int r = JOptionPane.showConfirmDialog(null, pf, title, JOptionPane.OK_CANCEL_OPTION);
        return r == JOptionPane.OK_OPTION ? pf.getPassword() : null;
    }

    /** 비ASCII 문자를 유니코드 이스케이프로 바꾼다. java.util.Properties 가 이 형태를 해석한다. */
    static String escape(char[] pw) {
        StringBuilder sb = new StringBuilder();
        for (char c : pw) {
            if (c < 0x80) sb.append(c);
            else sb.append(String.format("\\u%04x", (int) c));
        }
        return sb.toString();
    }

    static String prop(List<String> lines, String key) {
        for (String l : lines)
            if (l.startsWith(key)) {
                int i = l.indexOf('=');
                return i < 0 ? null : l.substring(i + 1).trim();
            }
        return null;
    }

    public static void main(String[] a) throws Exception {
        List<String> lines = Files.readAllLines(Paths.get(PROPS), StandardCharsets.UTF_8);
        String store = prop(lines, "RELEASE_STORE_FILE");
        String alias = prop(lines, "RELEASE_KEY_ALIAS");
        System.out.println("키스토어: " + store);
        System.out.println("alias   : " + alias);

        char[] sp = ask("키스토어(스토어) 비밀번호");
        if (sp == null) { System.out.println("취소됨"); return; }

        KeyStore ks = KeyStore.getInstance("PKCS12");
        try (InputStream in = new FileInputStream(store)) {
            ks.load(in, sp);
        } catch (IOException e) {
            System.out.println("✗ 스토어 비밀번호가 맞지 않습니다: " + e.getMessage());
            return;
        }
        System.out.println("✓ 스토어 열기 성공. alias 목록: " + Collections.list(ks.aliases()));

        char[] kp = ask("키(" + alias + ") 비밀번호  — 스토어와 같으면 같은 값 입력");
        if (kp == null) { System.out.println("취소됨"); return; }
        try {
            if (ks.getKey(alias, kp) == null) {
                System.out.println("✗ alias '" + alias + "' 를 찾을 수 없습니다.");
                return;
            }
        } catch (Exception e) {
            System.out.println("✗ 키 비밀번호가 맞지 않습니다: " + e.getClass().getSimpleName());
            return;
        }
        System.out.println("✓ 키 읽기 성공.");

        // gradle.properties 갱신 (비ASCII 문자는 유니코드 이스케이프로)
        Files.copy(Paths.get(PROPS), Paths.get(PROPS + ".bak"), StandardCopyOption.REPLACE_EXISTING);
        List<String> out = new ArrayList<>();
        boolean sawStorePw = false, sawKeyPw = false;
        for (String l : lines) {
            if (l.startsWith("RELEASE_STORE_PASSWORD")) {
                out.add("RELEASE_STORE_PASSWORD=" + escape(sp)); sawStorePw = true;
            } else if (l.startsWith("RELEASE_KEY_PASSWORD")) {
                out.add("RELEASE_KEY_PASSWORD=" + escape(kp)); sawKeyPw = true;
            } else out.add(l);
        }
        // 줄이 아예 없으면 새로 추가한다 (지워진 경우 대비)
        if (!sawStorePw) out.add("RELEASE_STORE_PASSWORD=" + escape(sp));
        if (!sawKeyPw)   out.add("RELEASE_KEY_PASSWORD=" + escape(kp));
        Files.write(Paths.get(PROPS), out, StandardCharsets.ISO_8859_1);
        System.out.println("\n✓ " + PROPS + " 갱신 완료 (백업: gradle.properties.bak)");
        System.out.println("  비ASCII 문자는 \\uXXXX 로 저장했습니다 — Gradle이 ISO-8859-1로 읽어도 안전합니다.");
        System.out.println("  이제 Android Studio에서 다시 빌드하세요.");
    }
}
