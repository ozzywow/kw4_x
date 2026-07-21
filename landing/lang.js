/* 한국어 / English 전환.
   - <head>에서 바로 실행되어 첫 페인트 전에 data-lang을 확정한다(깜빡임 방지).
   - 본문은 [lang="ko"] / [lang="en"] 요소를 나란히 두고 CSS로 한쪽만 보여준다.
   - 선택은 localStorage에 저장되어 세 페이지에서 함께 유지된다. */
(function () {
  var KEY = "kw4_lang";
  var root = document.documentElement;

  function stored() {
    try { return localStorage.getItem(KEY); } catch (e) { return null; }
  }
  function save(l) {
    try { localStorage.setItem(KEY, l); } catch (e) {}
  }

  function apply(l) {
    root.setAttribute("data-lang", l);
    root.setAttribute("lang", l);
    var btn = document.getElementById("langBtn");
    if (btn) {
      btn.textContent = l === "ko" ? "English" : "한국어";
      btn.setAttribute("aria-label", l === "ko" ? "Switch to English" : "한국어로 보기");
    }
  }

  // 우선순위: ?lang=ko|en (링크로 공유 가능) → 저장된 선택 → 브라우저 언어
  var q = /[?&]lang=(ko|en)/i.exec(location.search);
  var lang = (q && q[1].toLowerCase()) || stored() ||
             (/^ko/i.test(navigator.language || "") ? "ko" : "en");
  if (q) save(lang);
  apply(lang);

  document.addEventListener("DOMContentLoaded", function () {
    apply(lang);   // 버튼은 DOM 준비 후에야 잡힌다
    var btn = document.getElementById("langBtn");
    if (!btn) return;
    btn.addEventListener("click", function () {
      lang = lang === "ko" ? "en" : "ko";
      save(lang);
      apply(lang);
    });
  });
})();
