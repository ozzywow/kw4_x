// 키스토어 비밀번호 복구 시도.
//
// 가설: gradle.properties 에 적힌 한글 비밀번호가 "머릿속의 비밀번호"이고,
//       키스토어를 만들 때는 IME가 영문 상태여서 실제로는 그 한글의
//       두벌식 자판 키 입력(예: 한글떼기 -> gksrmfEortl)이 저장됐을 수 있다.
//       반대 경우도 있으므로 두 방향과 대소문자 변형까지 함께 시도한다.
//
// 찾은 비밀번호는 콘솔에 찍지 않고 GUI 창으로만 보여준다.
// 성공하면 gradle.properties 를 Gradle이 읽을 수 있는 형태로 고쳐 쓴다(.bak 백업).

import javax.swing.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.security.KeyStore;
import java.util.*;

public class RecoverPw {

    static final String PROPS = System.getProperty("user.home") + "\\.gradle\\gradle.properties";

    static final String CHO  = "ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ";
    static final String JUNG = "ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ";
    static final String JONG = " ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅅㅆㅇㅈㅊㅋㅌㅍㅎ";

    static final Map<Character, String> KEY = new HashMap<>();
    static {
        String[][] m = {
            {"ㄱ","r"},{"ㄲ","R"},{"ㄴ","s"},{"ㄷ","e"},{"ㄸ","E"},{"ㄹ","f"},{"ㅁ","a"},
            {"ㅂ","q"},{"ㅃ","Q"},{"ㅅ","t"},{"ㅆ","T"},{"ㅇ","d"},{"ㅈ","w"},{"ㅉ","W"},
            {"ㅊ","c"},{"ㅋ","z"},{"ㅌ","x"},{"ㅍ","v"},{"ㅎ","g"},
            {"ㅏ","k"},{"ㅐ","o"},{"ㅑ","i"},{"ㅒ","O"},{"ㅓ","j"},{"ㅔ","p"},{"ㅕ","u"},
            {"ㅖ","P"},{"ㅗ","h"},{"ㅘ","hk"},{"ㅙ","ho"},{"ㅚ","hl"},{"ㅛ","y"},{"ㅜ","n"},
            {"ㅝ","nj"},{"ㅞ","np"},{"ㅟ","nl"},{"ㅠ","b"},{"ㅡ","m"},{"ㅢ","ml"},{"ㅣ","l"},
            {"ㄳ","rt"},{"ㄵ","sw"},{"ㄶ","sg"},{"ㄺ","fr"},{"ㄻ","fa"},{"ㄼ","fq"},
            {"ㄽ","ft"},{"ㄾ","fx"},{"ㄿ","fv"},{"ㅀ","fg"},{"ㅄ","qt"},
        };
        for (String[] e : m) KEY.put(e[0].charAt(0), e[1]);
    }

    /** 한글 문자열 -> 두벌식 자판을 그대로 두드렸을 때 나오는 영문 */
    static String hangulToQwerty(String s) {
        StringBuilder sb = new StringBuilder();
        for (char c : s.toCharArray()) {
            if (c >= 0xAC00 && c <= 0xD7A3) {
                int i = c - 0xAC00;
                sb.append(KEY.getOrDefault(CHO.charAt(i / 588), "?"));
                sb.append(KEY.getOrDefault(JUNG.charAt((i % 588) / 28), "?"));
                char jong = JONG.charAt(i % 28);
                if (jong != ' ') sb.append(KEY.getOrDefault(jong, "?"));
            } else if (KEY.containsKey(c)) {
                sb.append(KEY.get(c));               // 낱자 자모
            } else {
                sb.append(c);                         // 영문/숫자/기호는 그대로
            }
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

    static boolean opens(String store, String pw) {
        for (String type : new String[]{"PKCS12", "JKS"}) {
            try (InputStream in = new FileInputStream(store)) {
                KeyStore.getInstance(type).load(in, pw.toCharArray());
                return true;
            } catch (Exception ignored) { }
        }
        return false;
    }

    static String escape(String pw) {
        StringBuilder sb = new StringBuilder();
        for (char c : pw.toCharArray())
            sb.append(c < 0x80 ? String.valueOf(c) : String.format("\\u%04x", (int) c));
        return sb.toString();
    }

    public static void main(String[] a) throws Exception {
        List<String> lines = Files.readAllLines(Paths.get(PROPS), StandardCharsets.UTF_8);
        String store = prop(lines, "RELEASE_STORE_FILE");
        String alias = prop(lines, "RELEASE_KEY_ALIAS");
        String sp = prop(lines, "RELEASE_STORE_PASSWORD");
        String kp = prop(lines, "RELEASE_KEY_PASSWORD");

        System.out.println("키스토어: " + store);
        System.out.println("alias   : " + alias);

        // 후보 생성 (값은 출력하지 않는다)
        LinkedHashMap<String, String> cands = new LinkedHashMap<>();
        for (String[] base : new String[][]{{"스토어값", sp}, {"키값", kp}}) {
            String name = base[0], v = base[1];
            if (v == null) continue;
            cands.put(name, v);
            String q = hangulToQwerty(v);
            cands.put(name + "→영문자판", q);
            cands.put(name + "→영문자판(소문자)", q.toLowerCase());
            cands.put(name + "→영문자판(대문자)", q.toUpperCase());
            cands.put(name + "(앞뒤공백제거)", v.trim());
        }

        String found = null, how = null;
        for (Map.Entry<String, String> e : cands.entrySet()) {
            System.out.print("  시도: " + e.getKey() + " ... ");
            if (opens(store, e.getValue())) {
                System.out.println("★ 열림!");
                found = e.getValue(); how = e.getKey();
                break;
            }
            System.out.println("실패");
        }

        if (found == null) {
            System.out.println("\n자동 복구 실패. 기억나는 후보를 직접 넣어보려면 KeystorePwFix 를 쓰세요.");
            return;
        }

        System.out.println("\n복구 성공 (" + how + "). 비밀번호는 화면 창으로만 표시합니다.");

        // 키 비밀번호도 같은 규칙일 가능성이 높으니 함께 확인
        String keyPw = found;
        try (InputStream in = new FileInputStream(store)) {
            KeyStore ks = KeyStore.getInstance("PKCS12");
            ks.load(in, found.toCharArray());
            try {
                ks.getKey(alias, found.toCharArray());
            } catch (Exception ex) {
                String alt = hangulToQwerty(kp);
                try { ks.getKey(alias, alt.toCharArray()); keyPw = alt; } catch (Exception ignored) { }
            }
        }

        Files.copy(Paths.get(PROPS), Paths.get(PROPS + ".bak"), StandardCopyOption.REPLACE_EXISTING);
        List<String> out = new ArrayList<>();
        for (String l : lines) {
            if (l.startsWith("RELEASE_STORE_PASSWORD")) out.add("RELEASE_STORE_PASSWORD=" + escape(found));
            else if (l.startsWith("RELEASE_KEY_PASSWORD")) out.add("RELEASE_KEY_PASSWORD=" + escape(keyPw));
            else out.add(l);
        }
        Files.write(Paths.get(PROPS), out, StandardCharsets.ISO_8859_1);
        System.out.println("gradle.properties 갱신 완료 (백업: gradle.properties.bak)");

        JTextArea ta = new JTextArea(
            "복구된 키스토어 비밀번호 (안전한 곳에 보관하세요)\n\n"
            + "스토어 비밀번호: " + found + "\n"
            + "키 비밀번호    : " + keyPw + "\n\n"
            + "gradle.properties 는 이미 갱신했습니다.");
        ta.setEditable(false);
        JOptionPane.showMessageDialog(null, ta, "복구 성공", JOptionPane.INFORMATION_MESSAGE);
    }
}
