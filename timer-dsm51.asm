
SEG_ON		EQU	P1.6	;w³¹czenie wyw.7-segm.
KEY		EQU	P3.5
LED		EQU	P1.7

;******* Pamiêæ wewnêtrzna RAM ********
KEY_SET		EQU	20H	;przekazanie danych o
				;klawiszach z przerwania
				;do programu g³ównego
FLAGA	EQU	KEY_SET+1	;zestaw flag bitowych
K_TEST EQU	FLAGA.0		;testowanie klawisza

DISPLAY		EQU	30H	;pamiêæ danych dla wyw.
LEDY	EQU	DISPLAY+6	;pamiêæ danych dla diod
KEY_LAST	EQU	LEDY+1 ;naciniête klawisze

STOS	EQU	60H
TIME	EQU	70H

;********* Ustawienie TIMERów *********
;TIMER 0
T0_G	EQU	0	;GATE
T0_C	EQU	0	;COUNTER/-TIMER
T0_M	EQU	1	;MODE (0..3)
TIM0	EQU	T0_M+T0_C*4+T0_G*8
;TIMER 1
T1_G	EQU	0	;GATE
T1_C	EQU	0	;COUNTER/-TIMER
T1_M	EQU	1	;MODE (0..3)
TIM1	EQU	T1_M+T1_C*4+T1_G*8

TMOD_SET	EQU	TIM0+TIM1*16

TH0_SET		EQU	256-4	;czas 4*256 = 1024 cykle
TL0_SET 	EQU	0	;= ok.1ms

TH1_SET		EQU	256-180
TL1_SET		EQU	0

;************** MACRA *****************
BANK0	MACRO		;ustawienie banku 0 rejestrów
	CLR	RS0	;(z banku 1)
	MACEND

BANK1	MACRO		;ustawienie banku 1 rejestrów
	SETB	RS0	;(z banku 0)
	MACEND
;**************************************

	LJMP	START

;********* Przerwanie Timer 0 *********
	ORG	0BH
	PUSH	PSW
	PUSH	ACC
	PUSH	B
	BANK1			;u¿ycie banku 1 rejestrów
	SJMP	PRZERWANIE_DALEJ

;********* Przerwanie Timer 1 *********
	ORG	1BH
	PUSH	PSW		;przechowanie rejestrów
	PUSH	ACC		;na stosie
	PUSH	B
	
	MOV	TH1,#TH1_SET		;TH1 na 50ms
	DJNZ	TIME,NO_1SEK		;czy minê³a 1 sek

	CJNE	@R0, #09H, INKREMENTACJA ;jesli przekroczy cyfre 9 w sekundach	
	MOV	@R0, #0

	MOV	R0,#DISPLAY+1	;adres danych
	CJNE	@R0, #05H, INKREMENTACJA ;jesli przekroczy 59 w sekundach
	MOV	@R0, #0
	
	MOV	R0, #DISPLAY+2	
	CJNE	@R0, #09H, INKREMENTACJA ;jesli przekroczy cyfre 9 w minutach
	MOV	@R0, #0
	

	MOV	R0, #DISPLAY+3	
	CJNE	@R0, #05H, INKREMENTACJA ;jesli przekroczy 59 w minutach
	MOV	@R0, #0
	
	MOV	R0, #DISPLAY+5
	MOV	A, @R0
	MOV	B, #10
	MUL	AB
	MOV	R0, #DISPLAY+4
	ADD	A, @R0
	CJNE	A, #12, INKR1
	MOV	R0, #DISPLAY+4
	MOV	@R0, #1
	MOV	R0, #DISPLAY+5
	MOV	@R0, #0
	
	CLR	LED	;w³aczenie lampki zmiany czasu z AM na PM			
	MOV	TIME,#20	;odczekaj kolejny czas
				;20*50ms=1s
	MOV	R0, #DISPLAY
	
NO_1SEK:
	POP	B
	POP	ACC		;na stosie
	POP	PSW
	RETI


INKR1:
	MOV	R0, #DISPLAY+4
	CJNE	@R0, #9, INKR1_01
	MOV	@R0, #0
	MOV	R0, #DISPLAY+5
	MOV	@R0, #1
	MOV	R0, #DISPLAY
	LJMP	NO_1SEK

INKR1_01:
	INC 	@R0
	MOV	R0, #DISPLAY
	LJMP	NO_1SEK

INKREMENTACJA:
	INC	@R0	;zwiekszam cyfre na wyswietlaczu
	MOV	TIME,#20	;odczekaj kolejny czas
				;20*50ms=1s
	MOV	R0, #DISPLAY
	SJMP NO_1SEK


;**************************************

PRZERWANIE_DALEJ:
	MOV	TH0,#TH0_SET  ;ustawienie TH0 na 1ms

	JB	K_TEST,KEY_ON ;czy klawisz odczytany
	JB	KEY,KEY_END	  ;jednoznacznie
KEY_OFF:
	MOV	A,R3		;klawisz zwolniony
	CPL	A
	ANL	KEY_LAST,A	;zeruj odpowiedni bit
	SJMP	KEY_END

KEY_ON:
	JNB	KEY,KEY_END
	MOV	A,R3		;klawisz naciniêty
	ANL	A,KEY_LAST
	JNZ	KEY_END		;klawisz by³ naciniêty

	MOV	A,R3		;klawisz by³ zwolniony
	ORL	KEY_LAST,A	;ustaw odpowiedni bit
	ORL	KEY_SET,A	;przeka¿ informacjê do
				;programu g³ównego

KEY_END:
	MOV	A,R3
	RL	A		;nastêpny wskanik (1z8)
	MOV	R3,A
	INC	R0		;adres danych dla
				;nastêpnego wskanika

	DJNZ	R2,DIS_END	;czy ostatni wskanik
				;tak - ustaw dane dla 1
	MOV	R0,#DISPLAY	;adres danych
	MOV	R3,#1		;wybór wskanika-kod 1z8
	MOV	R2,#7		;liczba wskaników
DIS_END:

	SETB	SEG_ON		;wy³¹czenie wskanika

	MOV	A,R3		;aktualny wskanik (1z8)
	MOVX	@R1,A		;wpisz do bufora wyboru

	INC	R1		;adres bufora danych
	MOV	A,@R0		;dane dla wskanika
	CJNE	R3,#40H,SEG	;czy 7 wskanik
	SJMP	LEDS		;tak-diody-nie ma zamiany
SEG:
	ACALL	GET_COD7	;zamiana danych na kod
LEDS:
	MOVX	@R1,A		;wpisanie kodu segmentów

	CLR	SEG_ON		;w³¹czenie wskanika
	DEC	R1		;adres bufora wyboru

	MOV	C,KEY		;wstêpny odczyt klawisza
	MOV	K_TEST,C	;przechowanie informacji
				;do nastêpnego przerwania
	POP	B
	POP	ACC
	POP	PSW
	RETI
;**************************************

GET_COD7:
	ANL	A,#0FH		;tabela kodów 0..15
	INC	A		;ominiêcie rozkazu RET
	MOVC	A,@A+PC		;pobranie kodu z tabeli
	RET

COD_7SEG:
	DB	03FH		;kod znaku 0
	DB	006H		;kod znaku 1
	DB	05BH		;kod znaku 2
	DB	04FH		;kod znaku 3
	DB	066H		;kod znaku 4
	DB	06DH		;kod znaku 5
	DB	07DH		;kod znaku 6
	DB	007H		;kod znaku 7
	DB	07FH		;kod znaku 8
	DB	06FH		;kod znaku 9
	DB	03FH		;kod znaku A
	DB	07CH		;kod znaku b
	DB	039H		;kod znaku C
	DB	05EH		;kod znaku d
	DB	079H		;kod znaku E
	DB	071H		;kod znaku F

;**************************************
	ORG	100H
START:
	MOV	SP,#STOS

	CLR	A		;wpisanie 0	
	MOV	DISPLAY,A	;na wszystkie wskaniki
	MOV	DISPLAY+1,A
	MOV	DISPLAY+2,A
	MOV	DISPLAY+3,A
	MOV	DISPLAY+4,#2
	MOV	DISPLAY+5,#1

	MOV	KEY_LAST,A	;stan pocz¹tkowy klawiszy
	MOV	KEY_SET,A	;wszystkie zwolnione

	BANK1			;rejestry banku1
				;dla obs³ugi wywietlacza
	MOV	R0,#DISPLAY	;adres danych
	MOV	R1,#CSDS+7	;adres wyboru wskanika
				;z mo¿liwoci¹ zamiany na
				;adres danych wskanika
	MOV	R2,#7		;liczba wskaników
	MOV	R3,#1		;wybór wskanika-kod 1z8
	BANK0

	MOV	TMOD,#TMOD_SET		;Timer 0 liczy czas
	MOV	TH0,#TH0_SET		;Timer 0 na 1ms
	MOV	TL0,#TL0_SET
	SETB	TR0			;start Timera

	
	SETB	EA		;zezwolenie na przerwanie
	SETB	ET0		;od Timera 0


	;LCALL	LCD_CLR
	MOV	R0,#DISPLAY ;adres danych wyw.


LOOP:				;reakcja na klawisze
				;testowane w przerwaniu
	
	JBC	KEY_SET.0,K_ENTER
	JBC	KEY_SET.1,K_ESC
	;CJNE	R2, #1, PRZYCISKI
	;SJMP	LOOP
;PRZYCISKI:
	JBC	KEY_SET.2,K_PRAWO
	JBC	KEY_SET.3,K_GORA
	JBC	KEY_SET.4,K_DOLL
	JBC	KEY_SET.5,K_LEWO
	SJMP	LOOP

K_ENTER:		;zerowanie sekund

	MOV	DISPLAY,#0
	MOV	DISPLAY+1,#0
	SJMP 	LOOP
	
K_ESC:		
	CJNE 	R2, #0, WYLACZ	; flaga, jezeli nie s¹ równe to zatrzymaj czas
	SETB	TR1		; wlacza timer 1
	MOV	TIME,#20	;odczekaj czas 20*50ms=1s
	SETB	ET1		;od Timera 1
	MOV	R2, #1
	LJMP	LOOP

WYLACZ:
	MOV	R2, #0
	CLR	TR1	; wylacza timer 1
	CLR	ET1
	LJMP 	LOOP

K_LEWO:	
	CJNE	R2, #0, LOOP
	
	MOV	R0, #DISPLAY+2	
	CJNE	@R0, #00H, DEKR_MIN ;jesli przekroczy cyfre 0 w minutach
	MOV	@R0, #9

	MOV	R0, #DISPLAY+3	
	CJNE	@R0, #00H, DEKR_MIN ;jesli przekroczy 00 w minutach
	MOV	@R0, #5
	MOV	R0, #DISPLAY
	LJMP	LOOP

DEKR_MIN:
	DEC	@R0	;zmniejszam cyfre na wyswietlaczu

	MOV	R0, #DISPLAY
	LJMP LOOP
	

K_PRAWO:
	CJNE	R2, #0, LOOP
	MOV	R0, #DISPLAY+2	
	CJNE	@R0, #09H, INKR_MIN ;jesli przekroczy cyfre 9 w minutach
	MOV	@R0, #0

	MOV	R0, #DISPLAY+3	
	CJNE	@R0, #05H, INKR_MIN ;jesli przekroczy 59 w minutach
	MOV	@R0, #0
	MOV	R0, #DISPLAY
	LJMP	LOOP

INKR_MIN:
	INC	@R0	;zwiekszam cyfre na wyswietlaczu

	MOV	R0, #DISPLAY
	LJMP LOOP

K_DOLL:
	CJNE	R2, #0, LOOP
	LCALL	K_DOL
	LJMP	LOOP

K_GORA:			;zwiêkszenie stanu godzin o 1
	CJNE	R2, #0, LOOP
	MOV	R0, #DISPLAY+5
	MOV	A, @R0
	MOV	B, #10
	MUL	AB
	MOV	R0, #DISPLAY+4
	ADD	A, @R0
	CJNE	A, #12, INKR
	MOV	R0, #DISPLAY+4
	MOV	@R0, #1
	MOV	R0, #DISPLAY+5
	MOV	@R0, #0
	MOV	R0, #DISPLAY
	LJMP	LOOP
INKR:
	MOV	R0, #DISPLAY+4
	CJNE	@R0, #9, INKR_01
	MOV	@R0, #0
	MOV	R0, #DISPLAY+5
	MOV	@R0, #1
	MOV	R0, #DISPLAY
	LJMP	LOOP

INKR_01:
	INC 	@R0
	MOV	R0, #DISPLAY
	LJMP	LOOP

K_DOL:			;zmniejszenie stanu godzin o 1
	MOV	R0, #DISPLAY+5
	MOV	A, @R0
	MOV	B, #10
	MUL	AB
	MOV	R0, #DISPLAY+4
	ADD	A, @R0
	CJNE	A, #1, DEKR
	MOV	R0, #DISPLAY+4
	MOV	@R0, #2
	MOV	R0, #DISPLAY+5
	MOV	@R0, #1
	MOV	R0, #DISPLAY
	LJMP	LOOP

DEKR:
	MOV	R0, #DISPLAY+4
	CJNE	@R0, #0, DEKR_01
	MOV	@R0, #9
	MOV	R0, #DISPLAY+5
	MOV	@R0, #0
	MOV	R0, #DISPLAY
	LJMP	LOOP

DEKR_01:
	DEC	@R0
	MOV	R0, #DISPLAY
	LJMP	LOOP

