# DAC převodník

## Cíle projektu

Sestavit R2R 8bitový DAC převodník ovládaný keypadem o rozměrech 4x3 tlačítek. S výstupem na display a UART.
### Shrnutí funkcí
* Převodník DAC s analogovou částí rekonstrukčních filtrů
* Proměnlivá frekvence generovaného signálu 100-3000Hz
* Různé typy signálů: (sinus, pila, obdélník, trojúhelník, pulzy, DTMF)
	


## Popis Hardware

### Schéma digitální části

![Digi_schema](images/schema_digi.PNG )

Jádrem digitální části je mikrokontrolér ATMEGA 328P, který slouží k ovládání displaye, čtení z keypadu, ovládání R2R převodníku a komunikace přes UART. Jeho periferie jsou tedy klávesnice LCD display 16x2 znaků a R2R převodník. 
LCD display komunikuje v 4 bitovém módu pro ušetření pinů. Každý sloupec respektive tlačítko keypadu se čte s opakovací frekvencí 325 Hz.

### Schéma analogové části
![Digi_schema](images/schema_analog.PNG )

Analogová část se skládá ze samotného R2R ladderu, kde byly zvoleny hodnoty rezistorů 1 respektive 2 kOhm jako kompromis mezi zatížením pinů a rozumnou výstupní impedancí převodníku. Na nej je ihned navázán sledovač v podobě 
neinvertujícího zapojení operačního zesilovače, který slouží jako impedanční transformátor mezi R2R převodníkem a rekonstrukčními filtry.

### Návrh Rekonstrukčního filtru
Rekonstrukční filtr typu DP byl navržen nástrojem firmy Analog Devices Analog Filter Wizard. Byl navržen jako DP 4. řádu s Butterworth Beselovou aproximací. Méně strmé aproximace byly zvoleny z důvodu fázového zkreslení.
Jako mezní frekvence byla zvolena frekvence 10 kHz, což dává rezervu 8 kHz nad maximální fundamentální složkou generovaného signálu (2 kHz) a zároveň umožňuje potlačit výrazně repliku spektra nad fvz/2=31 kHz.

Výsledné charakteristiky:
![Digi_schema](images/amplitude_char.PNG )
![Digi_schema](images/Phase_char.PNG )

## Code description and simulations

*Write your text here*


## Video/Animation

*Write your text here*


## References

*Write your text here*
