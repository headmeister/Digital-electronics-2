# DAC převodník

## Cíle projektu

Sestavit R2R 8bitový DAC převodník ovládaný keypadem o rozměrech 4x3 tlačítek. S výstupem na display a UART.
### Shrnutí funkcí
* Převodník DAC s analogovou částí rekonstrukčních filtrů
* Proměnlivá frekvence generovaného signálu 100-3000Hz
* Různé typy signálů: (sinus, pila +, pila -, obdélník, trojúhelník,Plně usměrněný sinus, Polousměrněný sinus, EKG, DTMF)
	


## Popis Hardware

### Schéma digitální části

![Digi_schema](images/schema_digi.PNG )

Jádrem digitální části je mikrokontrolér ATMEGA 328P, který slouží k ovládání displaye, čtení z keypadu, ovládání R2R převodníku a komunikace přes UART. Jeho periferie jsou tedy klávesnice LCD display 16x2 znaků a R2R převodník. 
LCD display komunikuje v 4 bitovém módu pro ušetření pinů. Každý sloupec respektive tlačítko keypadu se čte s opakovací frekvencí 325 Hz.
![Ana_schema](images/schema_analog.PNG )

Analogová část se skládá ze samotného R2R ladderu, kde byly zvoleny hodnoty rezistorů 1 respektive 2 kOhm jako kompromis mezi zatížením pinů a rozumnou výstupní impedancí převodníku. Na nej je ihned navázán sledovač v podobě 
neinvertujícího zapojení operačního zesilovače, který slouží jako impedanční transformátor mezi R2R převodníkem a rekonstrukčními filtry.
### Návrh rekonstrukčního filtru
Pro návrh rekonstrukčního filtru byl využit nástroj Analog Devices Analog Filter Wizard (https://tools.analog.com/en/filterwizard/). Jedná se o filtr typu dolní propust 4. řádu s Butterworth Besselovou aproximací. Jako mezní frekvence byla stanovena frekvence 10 kHz, teoreticky bychom mohli jít s mezní frekvencí výše ale z praktických důvodů a kvůli fázovému zkreslení je dobré se od teoretické maximální hodnoty (fvz/2=31 kHz) vzdálit co nejvíce. Vzhledem k tomu že generované signály mají fundamentální frekvenci max 2 kHz jeví se 5násobný headroom jako dostatečný.
![Digi_schema](images/schema_digi.PNG )

## Code description and simulations

*Write your text here*


## Video/Animation

*Write your text here*


## References

*Write your text here*