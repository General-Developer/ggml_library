/* <!-- START LICENSE -->


This Software / Program / Source Code Created By Developer From Company GLOBAL CORPORATION
Social Media:

   - Youtube: https://youtube.com/@Global_Corporation 
   - Github: https://github.com/globalcorporation
   - TELEGRAM: https://t.me/GLOBAL_CORP_ORG_BOT

All code script in here created 100% original without copy / steal from other code if we copy we add description source at from top code

If you wan't edit you must add credit me (don't change)

If this Software / Program / Source Code has you

Jika Program ini milik anda dari hasil beli jasa developer di (Global Corporation / apapun itu dari turunan itu jika ada kesalahan / bug / ingin update segera lapor ke sub)

Misal anda beli Beli source code di Slebew CORPORATION anda lapor dahulu di slebew jangan lapor di GLOBAL CORPORATION!

Jika ada kendala program ini (Pastikan sebelum deal project tidak ada negosiasi harga)
Karena jika ada negosiasi harga kemungkinan

1. Software Ada yang di kurangin
2. Informasi tidak lengkap
3. Bantuan Tidak Bisa remote / full time (Ada jeda)

Sebelum program ini sampai ke pembeli developer kami sudah melakukan testing

jadi sebelum nego kami sudah melakukan berbagai konsekuensi jika nego tidak sesuai ? 
Bukan maksud kami menipu itu karena harga yang sudah di kalkulasi + bantuan tiba tiba di potong akhirnya bantuan / software kadang tidak lengkap


<!-- END LICENSE --> */
@echo off

pushd %~dp0
set models_path=%CD%
for %%d in (%~dp0..) do set root_path=%%~fd
popd

set argc=0
for %%x in (%*) do set /A argc+=1

set models=tiny.en tiny base.en base small.en small medium.en medium large-v1 large-v2 large-v3 large-v3-turbo

if %argc% neq 1 (
  echo.
  echo Usage: download-ggml-model.cmd model
  CALL :list_models
  goto :eof
)

set model=%1

for %%b in (%models%) do (
  if "%%b"=="%model%" (
    CALL :download_model
    goto :eof
  )
)

echo Invalid model: %model%
CALL :list_models
goto :eof

:download_model
echo Downloading ggml model %model%...

cd "%models_path%"

if exist "ggml-%model%.bin" (
  echo Model %model% already exists. Skipping download.
  goto :eof
)

PowerShell -NoProfile -ExecutionPolicy Bypass -Command "Start-BitsTransfer -Source https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-%model%.bin -Destination ggml-%model%.bin"

if %ERRORLEVEL% neq 0 (
  echo Failed to download ggml model %model%
  echo Please try again later or download the original Whisper model files and convert them yourself.
  goto :eof
)

echo Done! Model %model% saved in %root_path%\models\ggml-%model%.bin
echo You can now use it like this:
echo build\bin\Release\whisper-cli.exe -m %root_path%\models\ggml-%model%.bin -f %root_path%\samples\jfk.wav

goto :eof

:list_models
  echo.
  echo Available models:
  (for %%a in (%models%) do (
    echo %%a
  ))
  echo.
  exit /b
