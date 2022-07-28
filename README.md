<h1 align="center"> 간단한 강체 물리엔진 </h2>  

## 목표
 * 최소한의 3D 렌더링, 강체의 운동(충돌과 반응) 구현
<br>

## 제작 방법  
 * 최대한 기초부터 직접 구현
 * 우선 공부를 위해 다음 자료들을 참고하며 각각 구현하되, 이 후 렌더링과 물리의 필요 기능들을 통합해 하나의 물리엔진으로 구현 
   * 렌더링 : 주로 [C++ DirectX11 Engine Tutorial - Jpres](https://www.youtube.com/watch?v=gQIG77PfLgo&list=PLcacUGyBsOIBlGyQQWzp6D1Xn6ZENx9Y2) 영상을 보면서 구현
   * 물리 : 주로 Game Physics Engine Development - Ian Millington 책을 보면서 구현(렌더링 구현이전까지 결과는 Unity에서 확인)
<br>

## 언어 및 사용 라이브러리 
 * C++(DirectX11, ASSIMP, ImGui)
<br>

## 제작 과정  
```
2022.7.1.(금) ~ 2022.7.5.(화)
 - 주제 선정
```
```
2022.7.6.(수)
 - 렌더링 : 프로젝트 생성 및 DirectX 라이브러리 추가, 윈도우 창 띄우기 구현
```
```
2022.7.7.(목)
 - 렌더링 : 2D 삼각형 띄우기 구현
```
```
2022.7.8.(금)
 - 렌더링 : 3D 큐브 띄우기(Index 버퍼 활용) 구현
 - 물리 : 속도, 가속도에 따른 물체 위치값 변경 구현(회전운동은 제외, 중력 적용 후 작동하는 것 확인 완료)
```
```
2022.7.11.(월)
 - 렌더링 : 카메라 제어, FPS 타이머, ImGui 라이브러리 추가 및 테스트 창 띄우기 구현
```
```
2022.7.12.(화)
 - 렌더링 : ASSIMP 라이브러리 추가 및 모델 불러오기, 모델 여러개 띄우기 구현
```
```
2022.7.13.(수)
 - 렌더링 : 조명(Point Light이고 Specular는 없음) 구현
```
```
2022.7.14.(목)
 - 렌더링 : 2D Sprite 띄우기, 스텐실 마스크 구현(여기까지가 DirectX 튜토리얼 끝)
```
```
2022.7.15.(금)
 - 렌더링 : 전반적인 클래스 간 구조 변경 계획
 - 물리 : C#에 구현했던 속도, 가속도에 따른 물체 위치값 변경을 C++로 구현, 자체 수학 라이브러리 만들기 시작(현재 벡터, 행렬 기본연산까지 구현)
  * 이번주는 코드 정리랑 구조를 바꾸는데 너무 신경쓰고 있었던게 아닌가 싶긴 함. 당장 물리부분은 일주일 내내 보지도 못하고 있는데.. 다음주는 일단 물리 구현을 해놓고 코드 정리는 그 뒤에 하는 식으로 해야될 것 같음
```
```
2022.7.18.(월)
 - 물리 : 스프링, 부력 구현
  * 힘, 속도, 시간 등등 식마다 단위를 통일시켜서 조정해 줘야하는데 책에 쓰여있는 대로 일단 적당히 이해하고 구현하다 보니 결과가 좀 이상하다 느낄 때 값들을 제대로 조절할 수가 없음 시간이 좀 걸리더라도 최소한 단위들은 제대로 이해하고 통일하는게 필요할 듯
```
![Spring](https://user-images.githubusercontent.com/34128052/179560080-97e50322-2708-4488-a20b-326b394ad5c5.gif)
![bouyancy](https://user-images.githubusercontent.com/34128052/179559765-c32a298d-3b91-4848-943f-b6740a9cdec6.gif)

```
2022.7.19.(화)
 - 물리 : 충격(Impulse) 구현
```
```
2022.7.20.(수)
 - 물리 : 충돌 해결 구현 중
```
```
2022.7.21.(목)
 - 물리 : 충돌 해결, 수학 라이브러리 구현
```
```
2022.7.20.(금)
 - 물리 : 쿼터니언 회전 구현 중
```
```
2022.7.25.(월)
 - 물리 : 쿼터니언 회전 구현(회전량에 오차가 있어서 다시한번 보는 중, 아직 원인 못찾음), 충돌 검출 구현 중
```
```
2022.7.26.(화)
 - 물리 : 쿼터니언 회전 구현
  * 회전량 오차의 문제점은 첫번째로 라디안 디그리 각 변환을 해줘야 하는데 이걸 몰랐고(머쓱ㅎ) 책에 오타가 있었음. 지금 찝찝한 부분은 Game Physics Engine Development 책에 대부분 의지해서 구현하기에는 점점 책의 순서가 뒤죽박죽으로 변해가고 오타가 너무 많다는것(책에도 순서가 난장판이라 미안하다 되어있긴한데 이게 생각보다 더 혼란을 일으켜서 많은 시간을 잡아먹음.. 또 코드는 해당 저자가 깃허브에 올린 코드랑 다름.. 특히 쿼터니언 합 계산에서 책에 써놓은것과 부호가 달라서 이거 잡느라 너무 많은 시간을 썼는데 결국 깃허브 코드가 맞았음) 그래서 앞으로는 위키백과를 주로(지금까지 나타난 문제들과 구현한것도 사실상 위키백과를 보고 거의다 고쳐냈음) 보며 구현할 계획
```
