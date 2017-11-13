# AG_extspot-v2-bayesian-model-evaluator

Il tool calcola le distribuzioni normali di features che descrivono i blobs di classe source e quelli di classe background. Si utilizza la classe `BlobsFinder` per estrarre i `Blob` dalle immagini del training set. Un `Blob` estratto da un'immagine simulata con flusso, viene etichettato come flusso se e solo se:
* è centrato
* contiene un numero di fotoni > 1
* è il primo blob etichettato come flusso per un'immagine x

## Input
* Path al training test (la cartella del training set deve contenere due sottocartelle `bg` e `flux`)
* CDELT1
* CDELT2
* PSF

`./bin/AG_extspot-v2-bayesian-model-evaluator TrainingSetShortTerm 0.5 0.5 4`

## Output
* BACKGROUND
  * AR  37.3845 16.5874
  * PH  1.22337 0.560462
  * PC  0.730668 1.17095
  * PM  13.6849 2.24028

* FLUX
  * AR  100.483 35.8895
  * PH  4.14473 1.96385
  * PC  2.77525 1.2273
  * PM  9.37673 1.43904

* Legend: 
  * AR  AREA (degrees)
  * PH  PHOTONS (number of photons)
  * PC  PHOTONS CLOSENESS (degrees)
  * PM  PIXELS MEAN (grey level)

## Aggiunta di un attributo
1) Aggiungere il calcolo dell'attributo nel costruttore della classe `Blob` (con aggiunta di getter)
2) Richiamare il nuovo attributo in printMeanAndDeviation( ... )
