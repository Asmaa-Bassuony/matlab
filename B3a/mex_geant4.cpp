#include "G4Types.hh"
#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4AnalysisManager.hh"
#include "G4TScoreNtupleWriter.hh"

#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include <iostream>
#include <cstdlib>  // For malloc, calloc, and free

#include "mex.h"

// The entry point for MATLAB (MEX gateway)
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
    if (nrhs != 1) {
        mexErrMsgIdAndTxt("mex_geant4:inputError", "One input (macro file path) required.");
    }

    // Get input from MATLAB (macro file) with dynamic memory allocation
    const mwSize macroFileLength = mxGetN(prhs[0]) + 1; // Including null-terminator
    char* macroFile = (char*)malloc(macroFileLength * sizeof(char));
    if (!macroFile) {
        mexErrMsgIdAndTxt("mex_geant4:allocationError", "Memory allocation failed for macroFile.");
    }
    mxGetString(prhs[0], macroFile, macroFileLength);

    // Example usage of calloc (if needed for future arrays)
    // int* exampleArray = (int*)calloc(100, sizeof(int));
    // if (!exampleArray) {
    //     free(macroFile);  // Clean up before error
    //     mexErrMsgIdAndTxt("mex_geant4:allocationError", "Memory allocation failed for exampleArray.");
    // }

    // Initialize Geant4 run manager using factory
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    // Detector construction
    runManager->SetUserInitialization(new B3::DetectorConstruction());

    // Physics list
    runManager->SetUserInitialization(new B3::PhysicsList());

    // Action initialization
    runManager->SetUserInitialization(new B3a::ActionInitialization());

    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // Initialize G4 kernel
    runManager->Initialize();

    // Get UI manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Execute the macro file (control Geant4 via a macro)
    std::string command = "/control/execute ";
    command += macroFile;
    UImanager->ApplyCommand(command);

    // Clean up dynamically allocated memory
    free(macroFile);
    // free(exampleArray);  // Clean up example array if used

    delete visManager;
    delete runManager;
}
