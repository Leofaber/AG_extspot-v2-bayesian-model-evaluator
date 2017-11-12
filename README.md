# AG_extspot-v2-bayesian-model-evaluator

Il tool calcola le distribuzioni normali di features che descrivono i blobs di classe source e quelli di classe background. 

## Input
* Path al training test (la cartella del training set deve contenere due sottocartelle `bg` e `flux`)
* CDELT1
* CDELT2
* PSF

`./bin/AG_extspot-v2-bayesian-model-evaluator TrainingSetShortTerm 0.5 0.5 4`

## Output
* BACKGROUND
  * AR  48.487 32.8191
  * PH  1.62088 1.38281
  * PC  0.994316 1.53314
  * PM  12.9332 2.2947

* FLUX
  * AR  98.1401 35.3572
  * PH  4.19263 1.97705
  * PC  2.52706 1.15786
  * PM  9.33045 1.41381

* Legend: 
  * AR  AREA (degrees)
  * PH  PHOTONS (number of photons)
  * PC  PHOTONS CLOSENESS (degrees)
  * PM  PIXELS MEAN (grey level)

## Aggiunta di un attributo
* 1) Aggiungere il calcolo dell'attributo nel costruttore della classe `Blob` (co aggiunta di getter)
* 2) Richiamare il nuovo attributo in printMeanAndDeviation( ... )
