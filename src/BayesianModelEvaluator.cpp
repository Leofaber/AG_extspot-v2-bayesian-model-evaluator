#include "BayesianModelEvaluator.h"

BayesianModelEvaluator::BayesianModelEvaluator(){

}

void BayesianModelEvaluator::computeModel(string trainingSetPath, double CDELT1, double CDELT2, double PSF){

	
	string trainingSetBackgroundPath = trainingSetPath+"/bg";

	string trainingSetFluxPath = trainingSetPath+"/flux";


	// Vector that contains all the file names of the training set background images.
	vector<string> fileNamesBackground = FolderManager::getFileNamesFromFolder(trainingSetBackgroundPath);

	// Vector that contains all the file names of the training set flux images.
	vector<string> fileNamesFlux = FolderManager::getFileNamesFromFolder(trainingSetFluxPath);

 


    	cout << "\nAnalysis started: " << fileNamesBackground.size() + fileNamesFlux.size() << " files need to be analyzed." << endl;		

	// Get all training instances
	pair<vector<Blob *> , vector<Blob *> > bgAndfluxBlobs = BayesianModelEvaluator::getAllBlobsFromTrainingSet(fileNamesBackground,fileNamesFlux,  trainingSetBackgroundPath, trainingSetFluxPath, CDELT1, CDELT2, PSF);		


	cout << "* Computing distributions.. " << endl;
	// Compute and print distributions (Bayesian Model)
	BayesianModelEvaluator::printMeanAndDeviation("BACKGROUND", bgAndfluxBlobs.first);
	
	BayesianModelEvaluator::printMeanAndDeviation("FLUX", bgAndfluxBlobs.second);
	
	cout << "\nLegend: "<< endl;
	cout << "AR  "<< "AREA (degrees)" << endl;
	cout << "PH  "<< "PHOTONS (number of photons)" << endl;
	cout << "PC  "<< "PHOTONS CLOSENESS (degrees)" << endl;
	cout << "PM  "<< "PIXELS MEAN (grey level)" << endl;


	cout << "\n\n" << endl;


	// Clean heap
	BayesianModelEvaluator::cleanMemory(bgAndfluxBlobs.first);
	BayesianModelEvaluator::cleanMemory(bgAndfluxBlobs.second);

	/* END */
}


pair<vector<Blob *>,vector<Blob *>> BayesianModelEvaluator::getAllBlobsFromTrainingSet( 
							vector<string> bgFileNames,
							vector<string> fluxFileNames, 
							string trainingSetBackgroundPath,
							string trainingSetFluxPath,							 
							double CDELT1, 
							double CDELT2, 
							double PSF
){

	
	// bg blobs
	vector<Blob *> bgBlobs;

	// flux blobs
	vector<Blob *> fluxBlobs;
	
	cout << "* Look up for blobs in training set backgroud images.." << endl;
	// look up for blobs in trainingSetBackgroundPath
	for(vector<string>::iterator it=bgFileNames.begin() ; it < bgFileNames.end(); it++)
    	{

		// filepath
		string filename = *it;
		string fitsFilePath = trainingSetBackgroundPath + "/" + filename;
		
		// convert fits to int **
		CustomMap map = MapConverter::mapPathToIntPtr(fitsFilePath.c_str());
		
		// search blobs
		vector<Blob *> blobs = BlobsFinder::findBlobs(fitsFilePath, map.image, map.rows, map.cols, CDELT1, CDELT2);
	

		// add blobs to all blobs list
		bgBlobs.insert( bgBlobs.end(), blobs.begin(), blobs.end() );
 

	}
 

	cout << "* Look up for blobs in training set flux images.." << endl;
	// look up for blobs in trainingSetBackgroundPath
	for(vector<string>::iterator it=fluxFileNames.begin() ; it < fluxFileNames.end(); it++)
    	{

		// filepath
		string filename = *it;
		string fitsFilePath = trainingSetFluxPath + "/" + filename;
		
		// convert fits to int **
		CustomMap map = MapConverter::mapPathToIntPtr(fitsFilePath.c_str());
		
		// search blobs
		vector<Blob *> blobs = BlobsFinder::findBlobs(fitsFilePath, map.image, map.rows, map.cols, CDELT1, CDELT2);
	
		// handling background blobs found in flux images
		for(vector<Blob *>::iterator it = blobs.begin() ; it < blobs.end(); it++){

			Blob * b = *it;

			if( b->isCentered() && ( b->getNumberOfPhotonsInBlob()>1 ) )	
				// add blobs to flux blobs list
				fluxBlobs.push_back(b);
			else
				// add blobs to bg blobs list
				bgBlobs.push_back(b);
		}
 
	}
 
	
	cout << "* Number of background blobs: " << bgBlobs.size() <<endl;
	cout << "* Number of flux blobs: " << fluxBlobs.size() <<endl;
	
	return make_pair(bgBlobs,fluxBlobs);


}

void BayesianModelEvaluator::printMeanAndDeviation(string type, vector<Blob *>& allBlobs){
		
	// the attribute values vectors
	vector<double> area;
	vector<double> photons;
	vector<double> photons_closeness;
	vector<double> pixel_mean;

	// population the attribute values vectors
	for(vector<Blob*>::iterator i = allBlobs.begin(); i != allBlobs.end(); i++){
		
		Blob * b = *i;

 		area.push_back(b->getArea());
		photons.push_back(b->getNumberOfPhotonsInBlob());
		photons_closeness.push_back(b->getPhotonsCloseness());
		pixel_mean.push_back(b->getPixelsMean());

		/*
		cout << "\n" << endl;
		cout << "file " << b->getFilePath() << endl;
		cout <<"getArea "<< b->getArea() << endl;
		cout <<"getNumberOfPhotonsInBlob "<< b->getNumberOfPhotonsInBlob() << endl;
		cout <<"getPhotonsCloseness "<< b->getPhotonsCloseness() << endl;
		cout <<"getPixelsMean "<< b->getPixelsMean() << endl;
		
		getchar();
		*/
	}
	

	// computing normal distributions
	normal_distribution<double> AREA = computeNormalDistribution(area);

	normal_distribution<double> PHOTONS = computeNormalDistribution(photons);

	normal_distribution<double> PHOTONS_CLOSENESS = computeNormalDistribution(photons_closeness);

	normal_distribution<double> PIXEL_MEAN = computeNormalDistribution(pixel_mean);

	 
	// print mean and deviation
	cout << "\n*"<<type<<endl;
	cout << "AR  "<< AREA.mean() 			<< " " << AREA.stddev() 		<< endl;
	cout << "PH  "<< PHOTONS.mean() 		<< " " << PHOTONS.stddev() 		<< endl;
	cout << "PC  "<< PHOTONS_CLOSENESS.mean() 	<< " " << PHOTONS_CLOSENESS.stddev() 	<< endl;
	cout << "PM  "<< PIXEL_MEAN.mean() 		<< " " << PIXEL_MEAN.stddev() 		<< endl;

	
	
}

normal_distribution<double>  BayesianModelEvaluator::computeNormalDistribution(vector<double>& attributeValues){
	
	double mean = 0;
	double deviation = 0;
	int total = attributeValues.size();

	for(vector<double>::iterator i = attributeValues.begin(); i != attributeValues.end(); i++){
		mean += *i;
	}
	mean = mean/(double)total;


	for(vector<double>::iterator i=attributeValues.begin() ; i < attributeValues.end(); i++) {
            
		double term = pow(*i - mean, 2);
	        deviation += term;
        }
        deviation = sqrt(deviation/total);
	
	normal_distribution<double> nd(mean,deviation);
	return nd;
}

 	

void BayesianModelEvaluator::cleanMemory(vector<Blob*>& blobs){

	for(vector<Blob*>::iterator i = blobs.begin(); i != blobs.end(); i++){
		Blob * b = *i;		
		delete b;
	}
	blobs.clear();
}


