//Notendurchschnitt 
// Das Programm berechnet den Notendurchschnitt der Abschlussnote für den Bachelor Studiengang REE
// Der Benutzer kann in diesem Programm auswählen, ob er die vorgegebene Datei ausgeben möchte, die Noten eingeben möchte, den Notendurchschnitt berechnen möchte und ob der Benutzer die Noten in einer neuen Datei speichern möchte
// Anna Katharina Becker, Finn-Niklas Zymelka, Nele Hetzer
// Datum: 21.01.2024
// Version 13

#define _CRT_SECURE_NO_WARNINGS                 //Warnungen werden unterdrückt
#include <stdio.h>                              //Biblitotheken werden eingefügt 
#include <stdlib.h>
#include <string.h>

typedef struct sNds {                           //Struktur mit sNds als Notendurchschnitt erstellen
    char modul[100];                            //Array 100 Zeichen für Ausgabe der Module
    char kürzel[6];                             //Array 6 Zeichen pro Kuerzel
    int gewichtung;                             //Deklaration Gewichtung der Fächer 
    int note;                                   //Noten 
} tNds;

//Funktionsdeklaration der ausgelagerten Funktionen 
void modulausgabe(tNds* module, int anzahlModule);                  //Module werden ausgegeben              
void noteneingabe(tNds* module, int anzahlModule);                  //führt die Eingabe der Noten durch 
void berechnungabschlussnote(tNds* module, int anzahlModule);       //Berechnung Abschlussnote + Ausgabe
void csverstellen(tNds* module, int anzahlModule);                  //neue Datei wird erstellt

int main(void) {
    printf("\nDies ist ein Programm fuer Studierende des REE-Studiengangs, welches die Abschlussnote berechnet.\n");
    printf("Dafuer koennen Sie die Module ausgeben lassen, sowie ihre Noten eintragen. Ihre Dateien werden in einer neuen Datei gespeichert.\n");

    tNds* module = NULL;                                            //Zeiger auf ein array der struktur module
    FILE* pDatei;                                                   //Zeiger der auf Array pDatei
    char modul[100];



    pDatei = fopen("module.csv", "r");                              //Datei wird geöffnet, read
    if (pDatei == NULL) {                                           //wenn Datei NULL ist -> Fehler beim öffnen
        perror("Fehler beim oeffnen der Datei", 132);               //Ausgabe an den Nutzer 
        return 1;                                                   //Error Meldung -> Fehlercode 1
    }


    module = (tNds*)malloc(sizeof(tNds));                           //Speicherreservierung für die Module

    if (module == NULL) {                                           //Fehler bei Speicherreservierung 
        printf("Fehler beim Speicher allokieren\n");                //Fehlerausgabe an Nutzer 
        fclose(pDatei);                                             //schließen der Datei
        return 1;
    }

    int anzahlModule = 0;                                           //Funktionsdeklaration wird auf 0 gesetzt warum?
    fgets(modul, sizeof(modul), pDatei);                            //zeilenweise einlesen der Datei, Speicher reservieren für Module

    while (fgets(modul, sizeof(modul), pDatei) != NULL) {           //so lange wie Zeilen vorhanden, Schleife läuft durch


        // Einlesen der Daten in die Struktur
        strcpy(module[anzahlModule].modul, strtok(modul, ";"));     //einlesen der Module
        strcpy(module[anzahlModule].kürzel, strtok(NULL, ";"));     //einlesen der Kuerzel, Zerlegung in Token (Trennzeichen ";")

        module[anzahlModule].gewichtung = atoi(strtok(NULL, ";"));  //
        module[anzahlModule].note = atoi(strtok(NULL, ";"));        //


        // Dynamischer Speicher für alle weiteren Module allokieren

        tNds* temp = (tNds*)realloc(module, (anzahlModule + 2) * sizeof(tNds));
        if (temp == NULL) {
            printf("Fehler beim Neu-Allokieren des Speichers\n");
            free(module);
            fclose(pDatei);
            return 1;
        }
        module = temp;

        anzahlModule++;
    }


    fclose(pDatei);




    char option;

    do {

        printf("\nMenue:\n");                                                           //Überschrift menü
        printf("1. Ausgabe der Module fuer den REE_Studiengang\n");                     //Ausgabe an Nutzer zur Auswahl
        printf("2. Eingabe deiner Noten\n");                                            //Noten für jedes Modul können eingetragen werden 
        printf("3. Berechnung und Ausgabe deiner Abschlussnote\n");                     //Abschlussnote wird ausgegeben 
        printf("4. Speichern deiner Noten in eine CSV-Datei\n");                        //Noten werden in neuer Datei gespeichert 
        printf("0. Programm beenden\n");                                                //Programm wird beendet 


        int retVal;                                                                     //Rückgabewert
        printf("\nBitte waehlen Sie eine Option von 0 bis 4:");                         //Ausgabe an Nutzer, Aufforderung zum Auswählen
        retVal = scanf(" %c", &option);                                                 //einlesen der Benutzereingabe
        printf("\n");                                                                   //Absatz


        switch (option) {                                                               //ähnlich zu if/else , verschiedenes Vorgehen nach Auswahl
        case '1':                                                                       //bei Eingabe 1

            modulausgabe(module, anzahlModule);                                         //Module ausgeben
            break;                                                                      //nach Ausgabe stopp und neue Abfrage
        case '2':                                                                       //bei Eingabe 2

            noteneingabe(module, anzahlModule);                                         //Noten können eingegeben werden 
            break;
        case '3':                                                                       //Eingabe 3

            berechnungabschlussnote(module, anzahlModule);                              //Abschlussnote wird berechnet und ausgegeben
            break;
        case '4':                                                                       //Eingabe 4

            csverstellen(module, anzahlModule);                                         //neue Datei mit eingetragenen Noten 
            break;
        case '0':                                                                       // Eingabe 0

            printf("Das Programm wird beendet.\n");                                     //Progamm beenden
            break;
        default:                                                                        //keins der Auswahlmöglichkeiten, ungültige auswahl
            printf("Ungueltige Auswahl. Bitte geben Sie eine Zahl zwischen 0 und 4 ein.\n"); //Ausgabe an Nutzer 
        }

    } while (option != '0');                                                            //Neue abfrage bis 0 eingegeben wird, 0 zum beenden des Programms


    free(module);                                                                       //Speicher wird freigegeben

    return 0;                                                                           //main wird beendet      
}

void modulausgabe(tNds* module, int anzahlModule) {                                     //ausgelagerte Funktion zur Ausgabe der Module
    for (int i = 0; i < anzahlModule; i++) {
        printf("Modul: %s, Kuerzel: %s, Gewichtung:%d, Note: %d\n", module[i].modul, module[i].kürzel, module[i].gewichtung, module[i].note);
    }
}

void noteneingabe(tNds* module, int anzahlModule) {                                     //ausgelagerte Funktion -> Noten können eingegeben werden 
    for (int i = 0; i < anzahlModule; i++) {
        int note;                                                                       //temporärer Speicher 
        while (1) {                                                                     //wenn

            printf("Geben Sie die Note fuer das Modul '%s' (0-15) ein: ", module[i].modul); //Notenabfrage, Nutzer wird aufgefordert seine Noten einzutragen


            if (scanf("%d", &note) != 1) {                                              //
                printf("Ungueltige Eingabe. Bitte geben Sie eine ganze Zahl ein.\n");   //Ausgabe Nutzer ungültigte Eingabe 

                while (getchar() != '\n');
            }
            else if (note >= 0 && note <= 15) {

                module[i].note = note;
                break;
            }
            else {
                printf("Ungueltige Note. Bitte geben Sie eine Note zwischen 0 und 15 ein.\n"); //Ausgabe an Nutzer 

                while (getchar() != '\n');
            }
        }
    }
}

void berechnungabschlussnote(tNds* module, int anzahlModule) {      //ausgelagerte Funktion für Berechnung der Note


    float summederNoten = 0.0;                                         //
    int gesamtGewichtung = 0;

    for (int i = 0; i < anzahlModule; i++) {
        summederNoten += module[i].gewichtung * module[i].note;
        gesamtGewichtung += module[i].gewichtung;
    }


    double abschlussNote = (summederNoten / gesamtGewichtung);


    printf("\nSumme aller Punkte: %.2f\n", summederNoten);                     //Ausgabe Nutzer, Summe aller Punkte 


    printf("Abschlussnote (Leistungspunkte): %.3f\n", abschlussNote);          //Abschlussnote wird ausgegeben 


    char* notenalsText;                                                        //Definition der Variable

    if (summederNoten >= 4930) {                                               //Punkte größer/gleich 4930 -> Ausgabe
        notenalsText = "ausgezeichnet";
    }
    else if (summederNoten >= 4250) {                                          //Variable größer gleich Punktanzahl
        notenalsText = "sehr gut";
    }
    else if (summederNoten >= 3230) {
        notenalsText = "gut";
    }
    else if (summederNoten >= 2210) {
        notenalsText = "befriedigend";
    }
    else if (summederNoten >= 1700) {
        notenalsText = "ausreichend";
    }
    else {
        notenalsText = "nicht bestanden";
    }


    printf("Abschlussnote (Text): %s\n", notenalsText);                         //Ausgabe der Abschlussnote mit Note als Text
}

void csverstellen(tNds* module, int anzahlModule) {                             //Datei wird erstellt, zum speichern der Noten
    FILE* pDatei = fopen("aktualisierteNoten_module.csv", "w");                      //

    if (pDatei == NULL) {
        perror("Fehler beim Oeffnen der Datei");
        return;
    }


    fprintf(pDatei, "Modul;Kuerzel;Gewichtung;Note\n", 129);


    for (int i = 0; i < anzahlModule; i++) {
        fprintf(pDatei, "%s;%s;%d;%d\n", module[i].modul, module[i].kürzel,
            module[i].gewichtung, module[i].note);
    }


    fclose(pDatei);

    printf("Noten erfolgreich in aktualisierteNoten_module.csv gespeichert.\n");
}