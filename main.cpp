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


using namespace std;
using namespace LibBoard;

int main(int argc, char **argv) {
		
	// variable definition
		boost::shared_ptr<prm::RBN> rbnTest;
		Graph graph;
		float compt=0.0;
		prm::PRMDisplay* prmdisplay;
	///PRM definition

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
			
			/// B3 - Set Probabilities associated to nodes dependencies
			/*plProbValue probas1[] = {0.33, 0.34, 0.33, 0.65, 0.25, 0.1, 0.85, 0.1, 0.05, 0.05, 0.10, 0.85, 0.1, 0.25, 0.65, 0.33, 0.34, 0.33};
			rbnTest->setDistributionTable("Registration.Satisfaction", rbnTest->getParents("Registration.Satisfaction"), initVector(probas1, 18));

			plProbValue probas2[] = {0.3, 0.6, 0.1, 0.8, 0.1, 0.1, 0.1, 0.5, 0.4, 0.5, 0.4, 0.1};
			rbnTest->setDistributionTable("Registration.Grade", rbnTest->getParents("Registration.Grade"), initVector(probas2, 12));

			plProbValue probas3[] = {0.333, 0.334, 0.333, 0.01, 0.19, 0.8, 0.2, 0.4, 0.4, 0.8, 0.19, 0.01};
			rbnTest->setDistributionTable("Course.Rating", rbnTest->getParents("Course.Rating"), initVector(probas3, 12));

			plProbValue probas4[] = {0.333, 0.334, 0.333, 0.1, 0.2, 0.7, 0.2, 0.4, 0.4, 0.6, 0.3, 0.1};
			rbnTest->setDistributionTable("Student.Ranking", rbnTest->getParents("Student.Ranking"), initVector(probas4, 12));

			plProbValue probas5[] = {0.75, 0.25, 0.25, 0.75};
			rbnTest->setDistributionTable("Professor.Popularity", rbnTest->getParents("Professor.Popularity"), initVector(probas5, 4));

			plProbValue probas6[] = {0.75, 0.25, 0.25, 0.75};
			rbnTest->setDistributionTable("Professor.Popularity", rbnTest->getParents("Professor.Popularity"), initVector(probas5, 4));
			
			rbnTest->setUniformDistribution("Student.Intelligence");
			rbnTest->setUniformDistribution("Course.Difficulty");
			rbnTest->setUniformDistribution("Professor.TeachingAbility");
		*/

	// Displaying
	double indice =3;
	std::string mySvgFile, tmp = "";
	//for(int i=0;i<7;i++){
//Etape 1: création du graphe de contrainte
		mySvgFile = "test";
		//tmp = boost::lexical_cast<std::string>(i);
		mySvgFile.append(tmp);
		std::cout << "Graphe de contrainte\n";
		prmdisplay=new prm::PRMDisplay(rbnTest,graph); 
		prmdisplay->RBNToGraph_preComputedClassVertex(1,indice);

	// Etape 2: algorithme de placement
		std::cout << "Application Kamada\n";
		prmdisplay->usedKamada(50.0);
		std::cout << "score de croisement " << prmdisplay->getCrossingScore() << std::endl;
	
	// Etape 3: Création graphique
		std::cout << "Coordinates\n";
	
		prmdisplay->placeVertex();
		std::cout << "relation\n";
		prmdisplay->placeRelationnalLink(); 
		std::cout << "pro\n";
		prmdisplay->placeProbabilistLink();


	// Etape 4: Affichage en image vectorielle
		std::cout << "Save file\n";
		prmdisplay->display("C:/Users/Arrizh/Desktop/testPred", mySvgFile);
		
		//delete prmdisplay;
		indice += .5;
		//prmdisplay->~PRMDisplay();
	//}
	

	//(Anthony) First optional argument is now the google test ::testing::FLAGS_gtest_filter
	//			Allows to locally update the GTest filter without globally updating the code
	/*if(argc > 1){
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
	system("PAUSE");
	

	return 0;
}

