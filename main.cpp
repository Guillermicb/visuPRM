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

			/// A2 - Foreign Keys attributes
			prm::Attribute studentRef("Student", rankingDomain);
			prm::Attribute courseRef("Course", rankingDomain);
			prm::Attribute instructorRef("Instructor", rankingDomain);

			/// A3 - Classes definitions

			//Create classes
			prm::Class student("Student"), registration("Registration"), professor("Professor"), course("Course");
			
			//Add attributes (descriptive + FK)
			student.addAttribute(ranking);
			student.addAttribute(intelligence);

			registration.addAttribute(grade);
			registration.addAttribute(satisfaction);
			registration.addAttribute(studentRef);
			registration.addAttribute(courseRef);

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

			//Add classes
			schema->addClass(student);
			schema->addClass(registration);
			schema->addClass(course);
			schema->addClass(professor);
			

			//Define reference slots (FK constraints)
			/*schema->addReferenceSlot("Registration", "Student", "Student");
			schema->addReferenceSlot("Registration", "Course", "Course");*/
			schema->addReferenceSlot("Course", "Instructor", "Professor");
			
			//// B = RBN definition

			/// B1 - Auto build nodes from schema
			rbnTest = boost::shared_ptr<prm::RBN>(new prm::RBN(*schema));
			rbnTest->init();

			/// B2 - Set dependencies between nodes
			rbnTest->setParents("Course.Rating", "Course.Instructor>Professor.TeachingAbility, Course.Instructor>Professor.Popularity");
			//rbnTest->setParents("Registration.Grade",	"Registration.Course>Course.Difficulty, Registration.Student>Student.Intelligence");
			/*rbnTest->setParents("Course.Rating",		"MODE(~Registration.Course>Registration.Satisfaction)");
			rbnTest->setParents("Student.Ranking",		"MODE(~Registration.Student>Registration.Grade)");
			rbnTest->setParents("Registration.Satisfaction", "Registration.Course>Course.Instructor>Professor.TeachingAbility, Registration.Grade");
			
			/// B3 - Set Probabilities associated to nodes dependencies
			plProbValue probas1[] = {0.33, 0.34, 0.33, 0.65, 0.25, 0.1, 0.85, 0.1, 0.05, 0.05, 0.10, 0.85, 0.1, 0.25, 0.65, 0.33, 0.34, 0.33};
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

			//Etape 1: création du graphe de contrainte
    VertexIndexPropertyMap vertexIdPropertyMap = boost::get(&VertexProperties::index, graph);
	PositionMap positionMap = boost::get(&VertexProperties::point, graph);


        VirtexDescriptor vd = boost::add_vertex(graph);

        vertexIdPropertyMap[vd] = "Professor.Popularity";
		positionMap[vd][0]=1;
		positionMap[vd][1]=1;
		
		vd = boost::add_vertex(graph);
        vertexIdPropertyMap[vd] = "Course.Rating";
		positionMap[vd][0]=5.0;
		positionMap[vd][1]=5.0;


		 vd = boost::add_vertex(graph);
        vertexIdPropertyMap[vd] = "Professor.TeachingAbility";
		positionMap[vd][0]=1;
		positionMap[vd][1]=8.0;
	
		vd = boost::add_vertex(graph);
        vertexIdPropertyMap[vd] = "Course.Instructor";
		positionMap[vd][0]=5.0;
		positionMap[vd][1]=1;
	   

		vd = boost::add_vertex(graph);
        vertexIdPropertyMap[vd] = "Professor.IDprof";
		positionMap[vd][0]=10;
		positionMap[vd][1]=8;
	
    
    boost::add_edge(boost::vertex(1, graph), boost::vertex(0, graph), EdgeProperty(5), graph);
    boost::add_edge(boost::vertex(2, graph), boost::vertex(0, graph), EdgeProperty(5), graph);

    std::cout << "Vertices\n";
    boost::print_vertices(graph, vertexIdPropertyMap);

    std::cout << "Edges\n";
    boost::print_edges(graph, vertexIdPropertyMap);

    
    WeightPropertyMap weightPropertyMap = boost::get(&EdgeProperty::weight, graph);
	/*
	// Etape 2: algorithme de placement
    boost::circle_graph_layout(graph, positionMap, 100);
   // boost::fruchterman_reingold_force_directed_layout(graph, positionMap, boost::square_topology<>());

    boost::kamada_kawai_spring_layout(graph, positionMap, weightPropertyMap,
        boost::square_topology<>(), boost::side_length<double>(10), boost::layout_tolerance<>(),
        1, vertexIdPropertyMap);
		//blabla
		*/
	
	// Etape 3: Création graphique
    std::cout << "Coordinates\n";
	prm::PRMDisplay* prmdisplay=new prm::PRMDisplay(rbnTest,positionMap,vertexIdPropertyMap, graph); 
	prmdisplay->placeVertex();
	prmdisplay->placeRelationnalLink(); 
	prmdisplay->placeProbabilistLink();

	// Etape 4: Affichage en image vectorielle
	prmdisplay->display("C:\\Users\\pierre\\Desktop", "test1");

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

