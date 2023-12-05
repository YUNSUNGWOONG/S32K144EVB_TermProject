# S32K144EVB_TermProject
마이크로프로세서 설계실험 TermProject - 엘리베이터제작하기

### 첫번째 ref코드>스위치와 LED가 연동되게 하기(linkSwitchToLED.c)
'SW1'누르면 누르면 error값 만큼 toggleLEDsInCycle()가 실행되게 하기<br>

층수의 차이만큼

'SW2'누르면 누르면 error값 만큼 toggleLEDsInCycle()가 실행되게 하기<br>
'SW3'누르면 누르면 error값 만큼 toggleLEDsInCycle()가 실행되게 하기<br>
'SW4'누르면 누르면 error값 만큼 toggleLEDsInCycle()가 실행되게 하기<br>
'SW5'누르면 누르면 error값 만큼 toggleLEDsInCycle()가 실행되게 하기<br>

### 두번째 ref코드>스위치와 세그먼트가 연동되게 하기(linkSwitchToSegmentDisplay.c)
처음에 1이 뜨도록 하고, 이후에 error값 만큼 줄어들거나 감소하게 하기<br>

<2-1>은 스위치없이 세그먼트값 출력하는 예제코드



### 세번째 ref코드> 스위치와 LCD가 연동되게 하기(linkSwitchToLCD.c)



### 네번째 ref코드> 스위치와 스텝모터 연동되게 하기(linkSwtichToDCMotor.c)

<4-1>은 가변저항(portc_15)과 연동되어 DC모터 제어하는 예제코드



### 다섯번째 ref코드> 스위취 연동을 인터럽트로 구현되게 하기(switchInterlockingWithInterrupts.c)

7주차 과제를 참고하여 첫번쨰 코드에서 구현된 결과물의 내부로직만 인터럽트 방식을 써서 구현되게 하기





### 마지막 ref코드> 위에서 구축한 코드 통합되게 하기(manila.c)

지금까지 만든 코드들을 하나로 누적해서 통합합니다.

이때 중복되는 코드들은 함수별로 순서화해서 정리하고 이들 코드들에 대해 별도의 서드파티(헤더파일)을 만들어 참조하는 형식을 취합니다.



