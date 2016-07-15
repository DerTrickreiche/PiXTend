# PiXTend
PixTend Firmware rewitten

Das ist ein vollständiger Rewrite der Firmware für den PiXtend-Microcontroller.
Die Kommunikation mit dem Raspberry Pi ist vollständig kompatibel zum Original.

Gegenüber dem Original gibt es die folgenden Änderungen:

DHT11
noch nicht implementiert

PWM/Servo
Nach dem Neustart befinden sich die PWM-Audgänge im PWM-Modus und bleiben auf LOW.
Die PWM-Frequenz kann direkt in Hz eingegeben werden.
Bei PWM 0 und 100% werden keine Nadelimpulse mehr generiert.

PWM_CTRL0[bit 0] ist invertiert.
PWM_CTRL1 -> LOW-Byte PWM-Frequenz
PWM_CTRL2 -> HIGH-Byte PWM-Frequenz


ADC
Die Smpling-Rate ist in ms einstellbar.
Das Ergebnis wird über einen gleitenden Mittelwert gebildet.
