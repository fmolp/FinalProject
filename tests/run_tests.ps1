$gcc = "gcc"
$proj = "..\\debtPayment.c"
$tests = @(
	@{ src = "test_display.c"; exe = "test_display.exe" },
	@{ src = "test_duplicate_ids.c"; exe = "test_duplicate_ids.exe" },
	@{ src = "test_add_id.c"; exe = "test_add_id.exe" },
	@{ src = "test_display_stdout.c"; exe = "test_display_stdout.exe" },
	@{ src = "test_error_handling.c"; exe = "test_error_handling.exe" }
)

if (Test-Path "..\debtPayment.csv") {
	Copy-Item -Path "..\debtPayment.csv" -Destination ".\debtPayment.csv" -Force
}

$allPassed = $true
foreach ($t in $tests) {
	$src = $t.src
	$exe = $t.exe
	Write-Host "Compiling $src -> $exe"
	& $gcc -DUNIT_TEST -o $exe $proj $src
	if ($LASTEXITCODE -ne 0) { Write-Host "Compile failed for $src"; $allPassed = $false; break }
	Write-Host "Running $exe"
	& (Join-Path (Get-Location) $exe)
	if ($LASTEXITCODE -ne 0) { $allPassed = $false }
}

if (Test-Path ".\debtPayment.csv") { Remove-Item ".\debtPayment.csv" }

if ($allPassed) { Write-Host "ALL TESTS PASSED"; exit 0 } else { Write-Host "SOME TESTS FAILED"; exit 1 }