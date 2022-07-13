# 간단한 강체 물리엔진  

### 목표
 **3D로 눈에 보이는 최소한의 렌더링, 강체간의 충돌과 반응**  
사실 너무 기초적인 수준이라 이미 구현된 코드도 많고 만들어도 저 말곤 딱히 쓸 사람도 없습니다  
다만 기초를 제대로 공부해야 겠다는 생각에 직접 이해하면서 구현해 보는데 그 의미를 두었습니다  
  
### 제작 방법
최대한 기초부터 직접 구현(책, 유튜브, 구글링 등 모든걸 활용)  
렌더링 : 주로 [C++ DirectX11 Engine Tutorial - Jpres](https://www.youtube.com/watch?v=gQIG77PfLgo&list=PLcacUGyBsOIBlGyQQWzp6D1Xn6ZENx9Y2) 영상을 보면서 구현  
물리 : 주로 Game Physics Engine Development - Ian Millington 책을 보면서 구현, 렌더링 구현 이전까지 결과는 Unity에서 확인  

### 프로젝트 환경  
언어 : C++  
사용 라이브러리 : DirectX11, ASSIMP, ImGui  

### 제작 과정  
```
2022.7.1.(금) ~ 2022.7.5.(화)
 - 주제 선정
```
```
2022.7.6.(수)
 - 렌더링 : DirectX 라이브러리 불러와서 프로젝트 생성, 윈도우 창 띄우기 구현
```
```
2022.7.7.(목)
 - 렌더링 : 2D 삼각형 띄우기 구현
```
```
2022.7.8.(금) ~ 2022.7.10.(일)
 - 렌더링 : 3D 큐브 띄우기(Index 버퍼 활용) 구현
 - 물리 : 속도, 가속도에 따른 물체 위치값 변경 구현(회전운동은 제외, 중력 적용 후 작동하는 것 확인 완료)
```
```
2022.7.11.(월)
 - 렌더링 : 카메라 제어, FPS 타이머, ImGui 라이브러리 설치 및 테스트 창 띄우기 구현
```
```
2022.7.12.(화)
 - 렌더링 : ASSIMP 라이브러리 설치 및 모델 불러오기, 모델 여러개 띄우기 구현, 불필요 코드 일부 정리
```
