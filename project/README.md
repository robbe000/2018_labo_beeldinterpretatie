# Project - Beamer Fun

## Opdracht
Je hebt een projector en een webcam die erop is gericht. Kan jij hier een spel maken die je kan bedienen met je handen of iets anders?

## Oplossingsmethode

* **Tracking** : Het tracken van één hand. Op deze manier de locatie van het hand in het beeld bepalen. 
OpenCv ondersteund acht soorten trackers. In de subfolder test_tracking probeer ik deze acht soorten uit en zal er gekozen worden voor de meest passende oplossing.

* **Detectie** : Voordat we een object kunnen volgen moeten we natuurlijk eerst weten welk object we moeten volgen.  We zullen een hand in het beeld proberen te vinden via een Viola–Jones object detector. Deze methode werd getest in de subfolder test_detectie.

* **Scherm detectie** : Initieel dacht ik dat het belangrijk zou zijn om te weten hoe groot het scherm in het beeld was. Ik heb hier veel testen mee gedaan en dit bleek van ondergeschikt belang te zijn. Daarom heb ik deze feature in het uiteindelijke project laten vallen. Meer info in de subfolder detect_scherm.

## Project
Het gehele project kan je terugvinden in de subfolder beamer_fun.
