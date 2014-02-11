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


typedef struct {
	double x; 
	double y; 
	double len;
	double hei;
	std::string classeName;
}  RectClass;

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
		void adjustDisplayAfterKamada();
		void displayKamadaCheck(bool);

		void addVertex(const std::string&, std::map<std::string, VertexDescriptor>&);
		void addVertex(const std::string&, std::map<std::string, VertexDescriptor>&, Graph& myGraph, PositionMap&);

		void addForeignKeyEdges_multiConnectedAttributs(std::map<std::string, VertexDescriptor>&, const double edgeWweight);
		void addForeignKeyEdges_artificialClassVertex(std::map<std::string, VertexDescriptor>&, const double edgeWweight);
		void addForeignKeyEdges_artificialClassVertex(std::map<std::string, VertexDescriptor>&, const std::map<std::string, double>&);
		void addForeignKeyEdges_artificialClassVertex_NumberOfLinks(std::map<std::string, VertexDescriptor>&, const double edgeWweight, std::map<std::string, unsigned int>& probabilisticConnection);
		
		
		/*!
		 *  \brief draw a relationnal link with bends
		 *	\param numCoude : a integer which represent the shape of the bends (cf functions)
		 *	\param x1 : x coordinate of the origin of the line
		 *	\param y1 : y coordinate of the origin of the line
		 *  \param x2 : x coordinate of the end of the line
		 *  \param y2 : y coordinate of the end of the line
		 */
		void drawRelationnalLink(int numCoude, double x1,double y1,double x2,double y2 );

		/*!
		 *  \brief draw a probabilist link link with bends
		 *	\param aggregat : the aggregat of the link. If there is no aggregat the value is "NO"
		 *	\param x1 : x coordinate of the origin of the arrow
		 *	\param y1 : y coordinate of the origin of the arrow
		 *  \param x2 : x coordinate of the end of the arrow
		 *  \param y2 : y coordinate of the end of the arrow
		 *	\param thickness: represent the thickness of the arrow calculate thnaks to the function calculateEdgeThickness();
		 */
		void drawProbabilistLink(const std::string aggregat, double x1,double y1,double x2,double y2,double thickness );

		inline double  min(double a,double b){return a<b?a:b;}
		inline double  max(double a,double b){return a>b?a:b;}
		void addProbabilistLink(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWweight);

		/*!
		 *	\brief permits to draw the bounding rectangle of all the individuals which belong the same class.
		 *	\param red : the red level of the bounding box color
		 *	\param green : the green level of the bounding box color
		 *	\param blue : the blue level of the bounding box color
		 */
		void drawClass(int red, int green, int blue);

		/*!
		 *	\brief Calculate the intersection between a ellipse and a line.
		 *	\param axe1 : the big radius of the ellipse
		 *	\param axe2 : the little radius of the ellipse
		 *	\param x1 : x coordinate of the origin of the line
		 *	\param y1 : y coordinate of the origin of the line
		 *  \param x2 : x coordinate of the end of the line
		 *  \param y2 : y coordinate of the end of the line
		 *	\param extremite: the origin or the end of the line.
		 *	\return: the x,y coordinates of the intersection's points 
		 */
		std::vector<double> intersectionEllispeLine(double axe1, double axe2, double x1, double y1, double x2, double y2, int extremite);

		/*!
		 *	\brief Calculate the size between the least and the utmost bounding on x and y.
		 *	\retun : the size of the screen (x,y)
		 */
		std::vector<double> displaySize();
		void addProbabilistLink(std::map<std::string, VertexDescriptor>& verticeContainer, const std::map<std::string, double>& probWeights);
		void addProbabilistLink_NumberOfLinks(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWweight, std::map<std::string, unsigned int>& probabilisticConnection);
		bool checkFkPkAttribute(const std::string&,const std::string&);
		void drawRelationnalLink(int nbCoude, double x1,double y1,double x2,double y2 );
		void drawProbabilistLink(const std::string aggregat, double x1,double y1,double x2,double y2 );
		std::pair<double, double> get2DLinearEquation(Points, Points) throw();
		Points& reversePointCoordinate(Points&);
		std::map<std::string, unsigned int> computeProbabilisticConnection();
		std::string concat2SortedClass(std::string, std::string);
		void usedKamadaWithoutCircleLayoutInit(const double sideLenght);
		double computeEdgeWeight(const std::string&, const std::string&, const std::map<std::string, double>&);
		void addAttributes(std::map<std::string, VertexDescriptor>&, const double, std::map<std::string, double>);
		std::string getVertexClassName(std::string vertexName);
		double computeNumberOfLinkWeight(double linkWeight, double numberOfLink);

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
		 *	\brief Ctor.
		 *	\param rbn : the RBN object which contains all the information
		 *	\param graph: the graph use by Kamada Kawai
		 */
		PRMDisplay();
		PRMDisplay(const boost::shared_ptr<prm::RBN> rbn,const Graph& graph);


		/*!
		 * \brief Dtor.
		 */
		 ~PRMDisplay();

		/*!
		 *	\brief permits to place and draw thanks to the function drawVertex() all the vertex in the svg file.
		 *	\param red : the red level of the vertex color
		 *	\param green : the green level of the vertex color
		 *	\param blue : the blue level of the vertex color
		 */
		void placeVertex(int red, int green, int blue);

		/*!
		 *	\brief permits to place and draw thanks to the function drawRelationnalLink() all the relational link in the svg file.
		 *	\param red : the red level of the Relationnal Link color
		 *	\param green : the green level of the Relationnal Link color
		 *	\param blue : the blue level of the Relationnal Link color
		 */
		void placeRelationnalLink(int red, int green, int blue);

		/*!
		 *	\brief permits to place and draw thanks to the function drawProbabilistLink() all the probabilistic link in the svg file.
		 *	\param red : the red level of the probabilistic link color
		 *	\param green : the green level of the probabilistic link color
		 *	\param blue : the blue level of the probabilistic link color
		 */
		void placeProbabilistLink(int red, int green, int blue);

		/*!
		 *	\brief permits to place and draw thanks to the function drawclass() all the bounding box in the svg files.
		 *	\param red : the red level of the bounding box color
		 *	\param green : the green level of the bounding box color
		 *	\param blue : the blue level of the bounding box color
		 */
		void placeClasse(int red, int green, int blue);
		
		/*!
		 *	\brief  Save the svg file.
		 *	\param path : represent the path wher the file is saved
		 *	\param name : represent the name of the path without the svg's extention
		 */
		void display(const std::string& path, const std::string& name);

		void RBNToGraph(const double attributeWeight, const double FKWeight);
		void RBNToGraph_AllAttributsConnected(const double attributeWeight, const double FKWeight, const double probWeight);
		void RBNToGraph_ArtificialClassVertex(const double attributeWeight, const double FKWeight, const double probWeight);
		void RBNToGraph_ArtificialClassVertex_NumberOfLink(const double attributeWeight, const double FKWeight, const double probWeight);
		void RBNToGraph_PreComputedClassVertex(const double sideLength, const double attributeWeight, const double probWeight);
		void usedKamada(const double sideLenght);

		int getNbCrossing();
		int getMaxCrossing();
		double getCrossingScore();
		void initGraph();
		std::pair<Points, Points> getExtremGraphPoint(Graph&);
		

	};

}

#endif //PRM_DISPLAY_H
