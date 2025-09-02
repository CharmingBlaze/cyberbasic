$ErrorActionPreference = "Stop"
# Prefer Ninja build if present; fallback to classic build
if (Test-Path "build-ninja/cyberbasic.exe") { $exe = "build-ninja\cyberbasic.exe" }
elseif (Test-Path "build/cyberbasic.exe") { $exe = "build\cyberbasic.exe" }
else {
  Write-Host "[ERROR] Could not find cyberbasic.exe in build-ninja or build. Please build first."; exit 1
}
$base = "tests"
$cases = @{
  "strings_ops.bas"            = "AB`nTRUE`nTRUE"
  "select_strings.bas"         = "group1"
  "select_numbers.bas"         = "mid"
  "select_case_relations.bas"  = "mid"
  "dim_2d_print.bas"           = "0,5"
  "io_readall_writeall.bas"    = "hello world"
  "while_for.bas"              = "0`n1`n2`n0`n1"
  "if_basic.bas"               = "eq"
  # REDIM tests
  "redim_min.bas"              = "1,2,3,,"
  "redim_preserve_test.bas"    = "1,2,3,,`n10,,"
  # agklite auto-detect + loops/continue demo
  "do_repeat_continue.bas"     = "D`n1`n3`n3"
}
$fails = 0
foreach ($t in $cases.Keys) {
  $p = Start-Process -FilePath $exe -ArgumentList "$base\$t" -NoNewWindow -PassThru -RedirectStandardOutput "stdout.tmp" -RedirectStandardError "stderr.tmp" -Wait
  $out = (Get-Content "stdout.tmp") -join "`n"
  $err = (Get-Content "stderr.tmp") -join "`n"
  if ($p.ExitCode -ne 0) { Write-Host "[FAIL] $t exit code $($p.ExitCode)"; $fails++; continue }
  if ($out -ne $cases[$t]) {
    Write-Host "[FAIL] $t"; Write-Host "  expected:`n$($cases[$t])"; Write-Host "  got:`n$out"; if ($err) { Write-Host "  stderr:`n$err" }; $fails++
  } else {
    Write-Host "[OK] $t"
  }
}
# negative case
$tneg = "strings_mixed_compare_negative.bas"
$pn = Start-Process -FilePath $exe -ArgumentList "$base\$tneg" -NoNewWindow -PassThru -RedirectStandardOutput "stdout.tmp" -RedirectStandardError "stderr.tmp" -Wait
$errn = (Get-Content "stderr.tmp") -join "`n"
if ($pn.ExitCode -eq 0 -or ($errn -notmatch "Type error: cannot compare string with non-string")) {
  Write-Host "[FAIL] $tneg negative case"; Write-Host "  stderr:`n$errn"; $fails++
} else { Write-Host "[OK] $tneg (negative)" }

# classic-mode negative: THEN-less single-line IF should error
$tneg2 = "classic_thenless_if_negative.bas"
$pn2 = Start-Process -FilePath $exe -ArgumentList "--dialect classic $base\$tneg2" -NoNewWindow -PassThru -RedirectStandardOutput "stdout.tmp" -RedirectStandardError "stderr.tmp" -Wait
$errn2 = (Get-Content "stderr.tmp") -join "`n"
if ($pn2.ExitCode -eq 0 -or ($errn2 -notmatch "THEN-less single-line IF is not allowed")) {
  Write-Host "[FAIL] $tneg2 classic negative"; Write-Host "  stderr:`n$errn2"; $fails++
} else { Write-Host "[OK] $tneg2 (classic negative)" }
Remove-Item stdout.tmp, stderr.tmp -ErrorAction SilentlyContinue
exit $fails
