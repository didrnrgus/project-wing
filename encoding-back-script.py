import os
import chardet

def detect_encoding(file_path):
    """파일의 현재 인코딩을 감지"""
    with open(file_path, 'rb') as f:
        raw_data = f.read()
        result = chardet.detect(raw_data)
        return result['encoding']

def convert_to_utf8(file_path):
    """파일을 UTF-8 형식으로 변환"""
    try:
        encoding = detect_encoding(file_path)
        if encoding is None:
            print(f"인코딩 감지 실패: {file_path}")
            return

        encoding = encoding.lower()
        if encoding == 'utf-8':  
            print(f"변환 불필요 (이미 UTF-8): {file_path}")
            return

        with open(file_path, 'r', encoding=encoding) as f:
            content = f.read()
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(content)

        print(f"변환 완료 ({encoding} → UTF-8): {file_path}")

    except Exception as e:
        print(f"변환 실패: {file_path} - 오류: {e}")

def process_directory():
    """디렉토리 내 .cpp, .h 파일을 UTF-8 으로 변환"""
    for root, dirs, files in os.walk("./KDT2Framework"):
        for file in files:
            if file.endswith(".cpp") or file.endswith(".h"):
                file_path = os.path.join(root, file)
                convert_to_utf8(file_path)

    for root, dirs, files in os.walk("./Common"):
        for file in files:
            if file.endswith(".cpp") or file.endswith(".h"):
                file_path = os.path.join(root, file)
                convert_to_utf8(file_path)

    for root, dirs, files in os.walk("./AssetProj"):
        for file in files:
            if file.endswith(".cpp") or file.endswith(".h"):
                file_path = os.path.join(root, file)
                convert_to_utf8(file_path)

if __name__ == "__main__":
    process_directory()
    print("모든 .cpp 및 .h 파일을 UTF-8 형식으로 변환 완료.")
