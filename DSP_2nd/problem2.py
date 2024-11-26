from PIL import Image
import numpy as np
from scipy.fft import fft2, ifft2
import os

def processing2(input_file, spectrum_file, output_file):

    img = Image.open(input_file) ## BMP 파일 열기
    img = img.convert("RGB")  # RGB로 변환
    rgb_data = np.array(img)  # NumPy 배열로 변환
    # 행 단위의 배열들을 담고 있는/ RGB 배열 3개를/ 요소로 하는 배열

    # 결과 데이터를 담을 행렬
    spectrum_data = []
    result_data = []

    for i in range(3):
        
        # DFT 생성
        channel = rgb_data[:, :, i] # 0 Red, 1 Green, 2 Blue
        fft_result = fft2(channel) # 2D 푸리에 변환
        

        # 필터 생성
        # 저주파 성분을 중앙으로, 고주파 성분을 가장자리로 이동
        fft_result = np.fft.fftshift(fft_result)

        # 중앙 위치 및 거리 계산
        rows, cols = fft_result.shape
        crow, ccol = rows//2, cols//2
        X, Y = np.meshgrid(np.arange(cols), np.arange(rows))
        distance = np.sqrt((X - ccol)**2 + (Y - crow)**2)

    
        # 로우 패스 필터 (원형 필터)
        radius = 30
        y, x = np.ogrid[:rows, :cols]
        low_pass = (distance <= radius)
        fft_result *= low_pass
        
        # 가우시안 필터
        gaussian = np.exp(-(distance**2) / (2 * (15**2)))
        fft_result *= gaussian

        # 성분 위치 복원
        fft_result = np.fft.ifftshift(fft_result)


        # 스펙트럼 생성
        magnitude_spectrum = np.log(1 + np.abs(fft_result)) # 로그 스케일
        normalized_spectrum = 255 * (magnitude_spectrum - np.min(magnitude_spectrum)) / (np.max(magnitude_spectrum) - np.min(magnitude_spectrum)) # 정규화
        spectrum_data.append(np.clip(normalized_spectrum, 0 , 255)) # 클리핑 & 데이터 저장
 

        # IDFT 생성
        ifft_result = ifft2(fft_result).real # 역푸리에 변환
        result_data.append(np.clip(ifft_result, 0, 255)) # 클리핑 & 데이터 저장


    # 데이터 병합
    spectrum = np.stack(spectrum_data, axis=-1).astype(np.uint8)
    result = np.stack(result_data, axis=-1).astype(np.uint8)


    # BMP로 저장
    spectrum_image = Image.fromarray(spectrum, mode="RGB")
    spectrum_image.save(spectrum_file)
    print(f"결과 파일이 {spectrum_file}에 저장되었습니다.")
    restored_image = Image.fromarray(result, mode="RGB")
    restored_image.save(output_file, "BMP")
    print(f"결과 파일이 {output_file}에 저장되었습니다.")


# 실행부
input_bmp = "twin_noise.bmp"  # 입력 BMP 파일 경로
spectrum_bmp = "spectrum.bmp" # 스펙트럼 BMP 파일 경로
output_bmp = "output.bmp"  # 출력 BMP 파일 경로

if os.path.exists(input_bmp):  # 입력 파일이 존재하는지 확인
    processing2(input_bmp, spectrum_bmp, output_bmp)
else:
    print(f"입력 파일 {input_bmp}이(가) 존재하지 않습니다.")