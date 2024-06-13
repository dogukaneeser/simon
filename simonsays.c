#include <msp430.h>
#include <stdlib.h>

#define ROT_BUTTON BIT5    // Button für Rot
#define GELB_BUTTON BIT4   // Button für Gelb
#define GRUN_BUTTON BIT3   // Button für Grün
#define BLAU_BUTTON BIT6   // Button für Blau

#define ROT_LED BIT0       // LED für Rot
#define GELB_LED BIT2      // LED für Gelb
#define GRUN_LED BIT3      // LED für Grün
#define BLAU_LED BIT4      // LED für Blau

volatile unsigned int i;             // Variable für Schleifen
volatile unsigned int button_zahl;   // Variable zur Speicherung der Tastennummer
volatile unsigned int widerholung[150]; // um Widerholung zu speichern
volatile unsigned int level = 0;     // Aktuelles Level

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Watchdog-Timer anhalten

    // LED-Konfigurationen
    P2DIR |= ROT_LED;  // Rot als Ausgang setzen
    P2OUT &= ~ROT_LED; // Rot ausschalten
    P2DIR |= GELB_LED; // Gelb als Ausgang setzen
    P2OUT &= ~GELB_LED; // Gelb ausschalten
    P2DIR |= GRUN_LED; // Grün als Ausgang setzen
    P2OUT &= ~GRUN_LED; // Grün ausschalten
    P2DIR |= BLAU_LED; // Blau als Ausgang setzen
    P2OUT &= ~BLAU_LED; // Blau ausschalten

    // Tasten-Konfigurationen
    P1DIR &= ~ROT_BUTTON; // Rot-Button als Eingang setzen
    P1DIR &= ~GELB_BUTTON; // Gelb-Button als Eingang setzen
    P1DIR &= ~GRUN_BUTTON; // Grün-Button als Eingang setzen
    P1DIR &= ~BLAU_BUTTON; // Blau-Button als Eingang setzen
    P1OUT |= ROT_BUTTON; // Pull-up für Rot-Button aktivieren
    P1OUT |= GELB_BUTTON; // Pull-up für Gelb-Button aktivieren
    P1OUT |= GRUN_BUTTON; // Pull-up für Grün-Button aktivieren
    P1OUT |= BLAU_BUTTON; // Pull-up für Blau-Button aktivieren
    P1REN |= ROT_BUTTON; // Pull-up/-down für Rot-Button aktivieren
    P1REN |= GELB_BUTTON; // Pull-up/-down für Gelb-Button aktivieren
    P1REN |= GRUN_BUTTON; // Pull-up/-down für Grün-Button aktivieren
    P1REN |= BLAU_BUTTON; // Pull-up/-down für Blau-Button aktivieren

    while(1){
        widerholung_erzeugen(); // Neue Schritt erzeugen
        zeig_widerholung(widerholung, level + 1); // die aktuelle Widerholung zeigen

        if(!widerholung_überprüfen(widerholung, level + 1)){ // Überprüfen die Eingaben des Benutzers
            alle_leds_blinken(); // Alle LEDs blinken lassen, wenn die Eingabe falsch ist
            level = 0; // Level zurücksetzen
        }
        else{
            level++; // Level erhöhen, wenn die Eingabe richtig ist
        }
    }

    return 0;
}

void delay(unsigned int ms){ // Verzögerungsfunktion
    while(ms--){
        __delay_cycles(1000); // 1 ms Verzögerung ist gleich zu dieser Funktion
    }
}

int button_lesen(void){
    while(1){
        if(!(P1IN & ROT_BUTTON)){ // Prüfen, ob die Rot-Taste gedrückt ist
            return 1;
        }
        if(!(P1IN & GELB_BUTTON)){ // Prüfen, ob die Gelb-Taste gedrückt ist
            return 2;
        }
        if(!(P1IN & GRUN_BUTTON)){ // Prüfen, ob die Grün-Taste gedrückt ist
            return 3;
        }
        if(!(P1IN & BLAU_BUTTON)){ // Prüfen, ob die Blau-Taste gedrückt ist
            return 4;
        }
    }
}

void led_blinken(unsigned int led){
    switch(led){
        case 1: // Rot
            P2OUT |= ROT_LED; // Rot-LED einschalten
            delay(500); // Verzögerung
            P2OUT &= ~ROT_LED; // Rot-LED ausschalten
            break;
        case 2: // Gelb
            P2OUT |= GELB_LED; // Gelb-LED einschalten
            delay(500); // Verzögerung
            P2OUT &= ~GELB_LED; // Gelb-LED ausschalten
            break;
        case 3: // Grün
            P2OUT |= GRUN_LED; // Grün-LED einschalten
            delay(500); // Verzögerung
            P2OUT &= ~GRUN_LED; // Grün-LED ausschalten
            break;
        case 4: // Blau
            P2OUT |= BLAU_LED; // Blau-LED einschalten
            delay(500); // Verzögerung
            P2OUT &= ~BLAU_LED; // Blau-LED ausschalten
            break;
    }
    delay(500); // Verzögerung zwischen den LEDs
}

void zeig_widerholung(volatile unsigned int *widerholung, unsigned int lange){
    for(i = 0; i < lange; i++){
        led_blinken(widerholung[i]); // Zeige jede LED in der Sequenz
    }
}

void widerholung_erzeugen() {
    widerholung[level] = (rand() % 4) + 1; // Neuen Schritt zur Sequenz hinzufügen
}

int widerholung_überprüfen(volatile unsigned int *widerholung, unsigned int lange){
    for(i = 0; i < lange; i++){
        button_zahl = button_lesen(); // Benutzer-Eingabe lesen
        if(button_zahl != widerholung[i]){ // Überprüfen, ob die Eingabe korrekt ist
            return 0; // Falsch
        }
        led_blinken(button_zahl); // Richtige Eingabe, LED blinken lassen
    }
    return 1; // Richtig
}

void alle_leds_blinken(void) {
    for(i = 0; i < 3; i++){
        P2OUT |= ROT_LED; // Alle LEDs einschalten
        P2OUT |= GELB_LED;
        P2OUT |= GRUN_LED;
        P2OUT |= BLAU_LED;
        delay(500); // Verzögerung
        P2OUT &= ~ROT_LED; // Alle LEDs ausschalten
        P2OUT &= ~GELB_LED;
        P2OUT &= ~GRUN_LED;
        P2OUT &= ~BLAU_LED;
        delay(500); // Verzögerung
    }
}
