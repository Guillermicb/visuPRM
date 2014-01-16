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
	float length; 
};

struct EdgeProperty
{
	EdgeProperty(){}
    EdgeProperty(const double &w):weight(w) {}
    double weight;
};


struct Dot
{
    int x;
    int y;
};

typedef boost::adjacency_list<boost::vecS,
            boost::vecS, boost::undirectedS,
            VertexProperties, EdgeProperty > Graph;


typedef boost::property_map<Graph, std::string VertexProperties::*>::type VertexIndexPropertyMap;
typedef boost::property_map<Graph, Points VertexProperties::*>::type PositionMap;
typedef boost::property_map<Graph, float VertexProperties::*>::type LengthMap;
typedef boost::property_map<Graph, double EdgeProperty::*>::type WeightPropertyMap;

typedef boost::graph_traits<Graph>::vertex_descriptor VertexDescriptor;


typedef struct ColorClass {
	int red; 
	int blue; 
	int green;
	std::string classeName;
};

typedef struct RectClass {
	float x; 
	float y; 
	float len;
	float hei;
	std::string classeName;
	#include <limits>;
};

namespace prm{

	class PRMDisplay{
	private: 
		Graph graph;
		boost::shared_ptr<prm::RBN> rbn;
		PositionMap  positionMap;
		Board board;
		VertexIndexPropertyMap vertexIdPropertyMap;
		LengthMap lengthmap;
		std::vector<ColorClass> listColor;
		std::vector<RectClass> listRect;
		float delta; // espace entre les attributs et le rectangle de la classe
		void adjustDisplayAfterKamada(const double lenght);
		void displayKamadaCheck(bool);
		void addVertex(const std::string&, std::map<std::string, VertexDescriptor>&);
		void addForeignKeyEdges_multiConnectedAttributs(std::map<std::string, VertexDescriptor>&, const double edgeWweight);
		void addForeignKeyEdges_artificialClassVertex(std::map<std::string, VertexDescriptor>&, const double edgeWweight);
		void addProbabilistLink(std::map<std::string, VertexDescriptor>&, const double edgeWweight);
		bool checkFkPkAttribute(const std::string&,const std::string&);
		void drawRelationnalLink(int nbCoude, float x1,float y1,float x2,float y2 );
		std::pair<double, double> get2DLinearEquation(Points, Points) throw();
		Points& reversePointCoordinate(Points&);

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
		void placeVertex();
		void placeRelationnalLink();
		void placeProbabilistLink();
		void placeClasse();
		void makeVertexColor(const std::string& colorname);
		void display(const std::string& path, const std::string& name);

		void RBNToGraph(const double attributeWeight, const double FKWeight);
		void RBNToGraph_AllAttributsConnected(const double attributeWeight, const double FKWeight, const double probWeight);
		void RBNToGraph_ArtificialClassVertex(const double attributeWeight, const double FKWeight, const double probWeight);
		void usedKamada(const double sideLenght);
		double distanceBetweenDot(float x1,float y1,float x2,float y2);
		inline double  min(float a,float b){return a<b?a:b;}
		double minDistance4point(float a,float b,float c,float d );
		void drawClass();

		int getNbCrossing();
		int getMaxCrossing();
		double getCrossingScore();
		void initGraph();

	};

}

#endif //PRM_DISPLAY_H
