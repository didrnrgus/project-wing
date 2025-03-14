@echo off
REM script.py 파일 실행 .bat 파일
REM Python 스크립트 실행 전 디렉토리 확인

SET PYTHON_EXEC=python
SET SCRIPT_PATH=encoding-back-script.py

%PYTHON_EXEC% %SCRIPT_PATH%
PAUSE
