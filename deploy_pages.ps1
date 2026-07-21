# landing/ 폴더의 현재 커밋 내용을 gh-pages 브랜치 루트로 배포한다.
#
#   사용법:  powershell -File deploy_pages.ps1
#
# 작업 트리를 건드리지 않고(체크아웃 없이) landing/ 의 트리만 떼어내
# gh-pages 커밋으로 만들어 push 한다. landing/ 변경은 먼저 커밋해 두어야 한다.

$ErrorActionPreference = "Stop"
Set-Location ($PSScriptRoot)

# 커밋되지 않은 landing/ 변경이 있으면 중단 (배포 내용은 항상 커밋된 상태여야 함)
$dirty = git status --porcelain -- landing
if ($dirty) {
    Write-Host "landing/ 에 커밋되지 않은 변경이 있습니다. 먼저 커밋하세요:" -ForegroundColor Yellow
    Write-Host $dirty
    exit 1
}

$tree = (git rev-parse "HEAD:landing").Trim()
$parent = (git rev-parse --verify --quiet gh-pages)

$msg = "GitHub Pages 배포 ($(git rev-parse --short HEAD))"
if ($parent) { $commit = (git commit-tree $tree -p $parent.Trim() -m $msg).Trim() }
else         { $commit = (git commit-tree $tree -m $msg).Trim() }

git update-ref refs/heads/gh-pages $commit
git push origin gh-pages
Write-Host "gh-pages 배포 완료: $commit" -ForegroundColor Green
