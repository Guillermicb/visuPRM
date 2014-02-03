/**
  * File : main.cpp
  * Description : Entry point for the test project. This file has been added here because there were multiple main functions 
  *               in the cpp files inside test folder and this was causing problem during building the project. 
  *               If you need to run specific test cases/tests, please modify this file as per the instruction below. 
  * Author : Rajani CHULYADYO
  * Creation Date : 18/04/2013
  */

#include <gtest\gtest.h>
#include <PRMDisplay.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <string>
#include "Board.h"
#include <RBN.h>
#include "Consts.h"
#include "RBNGenerateSchema.h"
#include "RBNGenerateDependencies.h"
#include "RBNGenerateParameters.h"
#include "Object.h"
#include <pl.h>
#include "RelationalSchema.h"

using namespace std;
using namespace LibBoard;

using namespace std;

int main(int argc, char **argv) {
	/*
	//(Anthony) First optional argument is now the google test ::testing::FLAGS_gtest_filter
	//			Allows to locally update the GTest filter without globally updating the code
	if(argc > 1){
		::testing::FLAGS_gtest_filter = argv[1];
	}

	::testing::InitGoogleTest(&argc, argv);
	int res;
	try {
		res = RUN_ALL_TESTS();
	}
	catch (std::exception &ex) 	{
		cerr << ex.what() << endl;
	}*/

	////// variable definition
		boost::shared_ptr<prm::RBN> rbnTest;
		Graph graph;
		float compt=0.0;
		prm::PRMDisplay* prmdisplay;

	////// generate PRMs
	/*	
		//// A = Relational Schema definition

			/// A1 - Descriptive Attributes

			//Satisfaction attribute
			plIntegerType oneToThreeType(1, 3);
			boost::shared_ptr<prm::Domain> satisfactionDomain = boost::shared_ptr<prm::Domain>();
			prm::Attribute satisfaction("Satisfaction", satisfactionDomain);
			
			boost::shared_ptr<prm::Domain> rankingDomain= boost::shared_ptr<prm::Domain>();
			prm::Attribute ranking			("Ranking",			rankingDomain);
			prm::Attribute intelligence		("Intelligence",	boost::shared_ptr<prm::Domain>());
			prm::Attribute grade			("Grade",			boost::shared_ptr<prm::Domain>());
			prm::Attribute difficulty		("Difficulty",		boost::shared_ptr<prm::Domain>());
			prm::Attribute teachingAbility	("TeachingAbility", boost::shared_ptr<prm::Domain>());
			prm::Attribute popularity		("Popularity",		boost::shared_ptr<prm::Domain>());
			prm::Attribute rating			("Rating",			boost::shared_ptr<prm::Domain>());
			prm::Attribute profID			("IDprof",			boost::shared_ptr<prm::Domain>());
			prm::Attribute courseID			("IDCourse",			boost::shared_ptr<prm::Domain>());
			prm::Attribute studentID		("IDStudent",			boost::shared_ptr<prm::Domain>());
			prm::Attribute regID		("IDRegistration",			boost::shared_ptr<prm::Domain>());

			/// A2 - Foreign Keys attributes
			prm::Attribute studentRef("Student", rankingDomain);
			prm::Attribute courseRef("Course", rankingDomain);
			prm::Attribute instructorRef("Instructor", rankingDomain);

			/// A3 - Classes definitions

			//Create classes
			prm::Class professor("Professor"), course("Course"), student("Student"), registration("Registration");
			
			//Add attributes (descriptive + FK)
			student.addAttribute(studentID);
			student.addAttribute(ranking);
			student.addAttribute(intelligence);
			registration.addAttribute(regID);
			registration.addAttribute(grade);
			registration.addAttribute(satisfaction);
			registration.addAttribute(studentRef);
			registration.addAttribute(courseRef);
			course.addAttribute(courseID);
			course.addAttribute(difficulty);
			course.addAttribute(rating);
			course.addAttribute(instructorRef);
			professor.addAttribute(profID);
			professor.addAttribute(teachingAbility);
			professor.addAttribute(popularity);

			// A4 - RelationalSchema object

			boost::shared_ptr<prm::RelationalSchema> schema(new prm::RelationalSchema());

			//Add primary Key
			std::vector<std::string> PKprof;
			PKprof.push_back("IDprof");
			professor.setPK(PKprof);
			std::vector<std::string> PKstudent;
			std::vector<std::string> PKcourse;
			PKcourse.push_back("IDCourse");
			course.setPK(PKcourse);
			PKstudent.push_back("IDStudent");
			student.setPK(PKstudent);
			
			std::vector<std::string> PKreg;
			PKreg.push_back("IDRegistration");
			registration.setPK(PKreg);


			//Add classes
			schema->addClass(student);
			schema->addClass(registration);
			schema->addClass(course);
			schema->addClass(professor);
			

			//Define reference slots (FK constraints)
			schema->addReferenceSlot("Registration", "Student", "Student");
			schema->addReferenceSlot("Registration", "Course", "Course");
			schema->addReferenceSlot("Course", "Instructor", "Professor");
			
			//// B = RBN definition

			/// B1 - Auto build nodes from schema
			rbnTest = boost::shared_ptr<prm::RBN>(new prm::RBN(*schema));
			rbnTest->init();

			/// B2 - Set dependencies between nodes
			rbnTest->setParents("Course.Rating", "Course.Instructor>Professor.TeachingAbility, Course.Instructor>Professor.Popularity");
			rbnTest->setParents("Registration.Grade",	"Registration.Course>Course.Difficulty, Registration.Student>Student.Intelligence");
			rbnTest->setParents("Course.Rating",		"MODE(~Registration.Course>Registration.Satisfaction)");
			rbnTest->setParents("Student.Ranking",		"MODE(~Registration.Student>Registration.Grade)");
			rbnTest->setParents("Registration.Satisfaction", "Registration.Course>Course.Instructor>Professor.TeachingAbility, Registration.Grade");
			
		/*/
		//1. First we have to generate the relational schema, the only needed parameter is the number of classes.
		RBNGenerateSchema gs(6);
		boost::shared_ptr<prm::RelationalSchema> schema=gs.generateSchema();
		
		//2. From this generated schema we will create an RBN object
		rbnTest=boost::shared_ptr<prm::RBN>(new prm::RBN(*(schema.get())));
		rbnTest->init();
		
		//3. We generate dependencies, the generate method has as parameters:
				//generates dependencies from max slot chain length= maxSlotChainLength
				//maximum induced Width=maxInducedWidth
				//maxParentsPerNode is the maximun number of parents per node, default 3
				//maxDepNumber is the maximum number of allowed relational dependencies, default 15
		RBNGenerateDependencies gd(rbnTest);
		gd.generate(3,2,4);
		
		//4. We finally add CPDs:
		RBNGenerateParameters gp(rbnTest);
		gp.computeAllCPTs();

	////// Displaying
	double indice=3;
	std::string mySvgFile, tmp = "";
	//for(int i=0;i<7;i++){
///Etape 1: création du graphe de contrainte
		mySvgFile = "test";
		//tmp = boost::lexical_cast<std::string>(i);
		mySvgFile.append(tmp);
		std::cout << "Graphe de contrainte\n";
		prmdisplay=new prm::PRMDisplay(rbnTest,graph); 
		prmdisplay->RBNToGraph_preComputedClassVertex(50.0,1.0,indice);
		//prmdisplay->RBNToGraph_ArtificialClassVertex(1.0,indice,indice);
		//prmdisplay->usedKamada(50.0);
	/// Etape 2: algorithme de placement
		std::cout << "Application Kamada\n";
		
		//std::cout << "score de croisement " << prmdisplay->getCrossingScore() << std::endl;
	
	/// Etape 3: Création graphique
		prmdisplay->placeVertex(255, 0,0);
		std::cout << "placeVertex done" << std::endl;
		prmdisplay->placeClasse(0,0,255);
		std::cout << "placeClasse done" << std::endl;
		prmdisplay->placeRelationnalLink(0,0,255); 
		std::cout << "placeRelationnalLink done" << std::endl;
		prmdisplay->placeProbabilistLink(255,0,0);
		std::cout << "placeProbabilistLink done" << std::endl;
		

	/// Etape 4: Affichage en image vectorielle
		std::cout<<"affichage";
		prmdisplay->display("C:/Users/Arrizh/Desktop/testPred", mySvgFile);
		
		//delete prmdisplay;
		indice += .5;
		//prmdisplay->~PRMDisplay();
	//}


	system("PAUSE");
	return 0;
}

