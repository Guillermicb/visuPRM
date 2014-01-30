/**
  * File : RBN.h
  * Description : This class header place and display the attributs ant the links extract form RBN
  * Author : Pierre GOURET & Brice GUILLERMIC
  * Creation Date : 23/11/2013
  */

#ifndef PRM_DISPLAY_H
#define PRM_DISPLAY_H
#include "RBN.h"
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/simple_point.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/kamada_kawai_spring_layout.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <Board.h>


using namespace LibBoard;
typedef boost::circle_topology<>::point_type Points;
typedef boost::circle_topology<> Topology;
struct VertexProperties
{
    std::string index;//Classes.Attribut
    Points point;
	double length; 
};

struct EdgeProperty
{
	EdgeProperty(){}
    EdgeProperty(const double &w):weight(w) {}
    double weight;
};


typedef boost::adjacency_list<boost::vecS,
            boost::vecS, boost::undirectedS,
            VertexProperties, EdgeProperty > Graph;


typedef boost::property_map<Graph, std::string VertexProperties::*>::type VertexIndexPropertyMap;
typedef boost::property_map<Graph, Points VertexProperties::*>::type PositionMap;
typedef boost::property_map<Graph, double VertexProperties::*>::type LengthMap;
typedef boost::property_map<Graph, double EdgeProperty::*>::type WeightPropertyMap;

typedef boost::graph_traits<Graph>::vertex_descriptor VertexDescriptor;


typedef struct RectClass {
	double x; 
	double y; 
	double len;
	double hei;
	std::string classeName;
};

namespace prm{

	class PRMDisplay{
	private: 
		//Attributes
		Graph graph;
		boost::shared_ptr<prm::RBN> rbn;
		PositionMap  positionMap;
		Board board;
		VertexIndexPropertyMap vertexIdPropertyMap;
		LengthMap lengthmap;
		std::vector<RectClass> listRect;
		double delta; // espace entre les attributs et le rectangle de la classe

		// Functions
		void adjustDisplayAfterKamada(const double lenght);
		void displayKamadaCheck(bool);
		void addVertex(const std::string&, std::map<std::string, VertexDescriptor>&);
		void addVertex(const std::string&, std::map<std::string, VertexDescriptor>&, Graph& myGraph, PositionMap&);
		void addForeignKeyEdges_multiConnectedAttributs(std::map<std::string, VertexDescriptor>&, const double edgeWweight);
		void addForeignKeyEdges_artificialClassVertex(std::map<std::string, VertexDescriptor>&, const double edgeWweight);
		void addForeignKeyEdges_artificialClassVertex(std::map<std::string, VertexDescriptor>&, const std::map<std::string, std::pair<double, double>>&);
		void addProbabilistLink(std::map<std::string, VertexDescriptor>&, const double edgeWweight);
		bool checkFkPkAttribute(const std::string&,const std::string&);
		void drawRelationnalLink(int nbCoude, double x1,double y1,double x2,double y2 );
		void drawProbabilistLink(const std::string aggregat, double x1,double y1,double x2,double y2 );
		std::pair<double, double> get2DLinearEquation(Points, Points) throw();
		Points& reversePointCoordinate(Points&);
		std::map<std::string, unsigned int> computeProbabilisticConnection();
		std::string concat2SortedClass(std::string, std::string);
		void prevKamada(const double sideLenght, Graph& myGraph, PositionMap&);
		double computeEdgeWeight(const std::string&, const std::string&, const std::map<std::string, std::pair<double, double>>&);

		/*!
		 * \return width and height from the graph bounding box
		 * \param a graph you want to compute
		 */
		std::pair<double, double> computeBoundingLength(Graph&);
		

		/*!
		 * \return TRUE if xcoordinate is between x1 and x2
		 * \param xcoordinate the tested point
		 * \param x1 and x2 : end of the interval
		 */
		bool isBetween(double xcoordinate, double x1, double x2);

		/*!
		 * \return the begin and the end coordinates point of the edge 
		 * \param a pointer to the edge
		 */
		std::pair<Points, Points> getEdgeCoordinates(boost::graph_traits<Graph>::edge_iterator);
		
	public:
		
		/*!
		 * \brief Ctor.
		 * \param schema : the RelationalSchema giving the domain and the structure of the problem to modelize
		 */
		PRMDisplay();
		PRMDisplay(const boost::shared_ptr<prm::RBN> rbn,const Graph& graph);
		PRMDisplay(const boost::shared_ptr<prm::RBN> rbn, const PositionMap& postionMap, const VertexIndexPropertyMap& vertexNamemap,const Graph& graph);
		PRMDisplay(const PositionMap& postionMap, const VertexIndexPropertyMap& vertexNamemap, const Graph& graph);

		/*!
		 * \brief Dtor.
		 */
		// ~PRMDisplay();
		void placeVertex(int red, int green, int blue);
		void placeRelationnalLink(int red, int green, int blue);
		void placeProbabilistLink(int red, int green, int blue);
		void placeClasse(int red, int green, int blue);
		//void makeVertexColor(const std::string& colorname);
		void display(const std::string& path, const std::string& name);

		void RBNToGraph(const double attributeWeight, const double FKWeight);
		void RBNToGraph_AllAttributsConnected(const double attributeWeight, const double FKWeight, const double probWeight);
		void RBNToGraph_ArtificialClassVertex(const double attributeWeight, const double FKWeight, const double probWeight);
		void RBNToGraph_preComputedClassVertex(const double attributeWeight, const double probWeight);
		void usedKamada(const double sideLenght);
		double distanceBetweenDot(double x1,double y1,double x2,double y2);
		inline double  min(double a,double b){return a<b?a:b;}
		inline double  max(double a,double b){return a>b?a:b;}
		double minDistance4point(double a,double b,double c,double d );
		void drawClass(int red, int green, int blue);
		std::vector<double> intersectionEllispeLine(double axe1, double axe2, double x1, double y1, double x2, double y2, int extremite);

		int getNbCrossing();
		int getMaxCrossing();
		double getCrossingScore();
		void initGraph();
		std::pair<Points, Points> getExtremGraphPoint(Graph&);
		std::vector<double> displaySize();

	};

}

#endif //PRM_DISPLAY_H
