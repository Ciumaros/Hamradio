# Hamradio
Proiect de Duminică 					

Ceas pentru concurs cu Arduino Nano și RTC


De ceva vreme încerc să-mi fac timp pentru un proiect destul de simplu dar în același timp util, la care mă gâdesc de ceva vreme și nu am avut timpul necesar să-l încep darămite să-l și duc până la capăt.
Aveam, prin șmelțurile mele, un arduino nano care a fost achiziționat datorită prețului și nu pentru un proiect anume, iar eu voiam ceva simplu, care să-mi ofere și oarece satisfacție. În ideea că nu am făcut nici măcar inițerea în arduino, teama era destulă dar nefondată. În sfârșit, mi-am pus la punct planul, am achiziționat piesele de care mai era nevoie, și m-am apucat de treabă, puțin copy/paste de aici putină documentare de acolo și uite așa am încropit și firmwareul ce urma să-l uploadez pe arduino. După ce am construit ceasul pe breadboard și am făcut câteva debuguri la firmware totul era pregătit pentru a începe construcția montajului final.

Astfel: am folosit în acest proiect următoarele componente și module:
1 x  Arduino Nano ,
1 x RTC cu comunicare I2c,
1 x LCD 16X2 compatibil HD44780, 
1 x adaptor I2c la LCD,
1 x R 10K ohm,
1 x R 100K ohm,
1 x Cablaj de test cu dimensiuni 60x80 (din acela cu găurele),
2 x Rigletă cu pini (tată) 1x20,
1 x Rigletă cu pini (mama) 1x20
1 x Baterie 9v
1 x cutie de tablă refolosită (destul de mare sa-ti incapa totul in ea),
1 x comutator ON/OFF, 
1 x Push Button,

Ca și scule, am avut următoarele:
1 x ciocan,
1 x șurubelniță „Phillip” („cruce” fară să știe mitropolitul că vrea drepturi de autor),
1 x pilă dreaptă
1 x letcon cu pământare(ca să nu ardem microcontrolerul cand facem reparații, GRIJA MARE CA SE ARDE),
1 x dremel cu pânză pentru tăiat fier,
1 x cutter,
1 x ciupitor,
1 x patent,
Fludorul, sacâzul și sârmele cred că le are toata lumea. 
 Să trecem la treabă: 
 
	
Am început prin a lipi adaptorul I2c la LCD pe un lcd16x2 de dimensiuni rezonabile...mai mare decât cele standard (recuperat, nu mai știu de pe unde). Pe placa de test am lipit baretele pentru arduino și pentru RTC conform schemei, pe aceeași placă am lipit și firele pentru conectarea lcd-ului, LCD-ul este unul recuperat, cu dimensiuni mari pentru o vizibilitate bună dar care este 100% compatibil cu HD44780, 16 caractere pe 2 rânduri. Apoi am lipit cele două rezistențe ce formează divizorul de tensiune pentru a coborî nivelul măsurat la sub 5v maxim măsurabil de arduino.
	Apoi am trasat, cu un marker, forma LCD-ului pe capacul superior al cutiei, urmând imediat și tăierea cu dremelul, apoi am modificat și lărgit găurile de pe spatele cutiei astfel încât să iasă mufele de alimentare și mufa USB a modulului arduino. Am punctat unde vin găurile pentru șuruburi. După ce am montat arduino în interiorul cutiei și mi-am făcut o idee unde și cum montez restul m-am apucat de lipit pinii pe PCB de test  urmând imediat și firele aferente conexiunilor.
	După cum se vede și din schemă, divizorul de tensiune, se leagă pe pinul A0 al modulului arduino, acesta fiind un pin ANALOG IN ce conține un DAC cu rezoluție de 10 biti, el oferindu-i procesorului o valoare între 0 si 1023, mai mult decât de ajuns pentru a avea o acuratete de 0.01v în măsurarea tensiunii bateriei.
	Ultima conexiune cu arduino se face pe pinul A2 unde s-a conectat un buton push cu celălalt capăt la GND.
	Alimentare din baterie se face pe pinul VIN al modulului, iar toate perifericele se conectează pe pinul +5v de pe arduino.
	Ceasul este gata de utilizare și nu mai este necesar decât să-i reglăm ora.
Pentru aceasta există o porțiune din cod unde se introduce data și ora exactă, UTC, și se „de-comentează” linia de cod semnalizată astfel dupa care facem upload, la cod, în arduino. După ce am văzut că arduino afișează data si ora corect, vom „re-comenta” linia de cod „de-comentată” anterior și facem iarăși upload la cod. Nefiind necesare alte modificări în cod până când vom sincroniza iarăși ceasurile (în funcție de modulul RTC folosit este posibil să nu mai fie necesară o astfel de sincronizare)	Butonul aprinde retro-ilumiarea ecranului, pentru economisirea bateriei, stingându-se la 10 secunde dupa eliberarea butonului.

	Baftă la construit.
