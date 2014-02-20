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
		/*!
		 * \brief translate the PRM to the top left-hand corner 
		 */
		void adjustDisplayAfterKamada();

		/*!
		 * \brief check if the kamada-kawai algorithm worked
		 * \param check : the result of the kamada-kawai algorithm
		 */
		void displayKamadaCheck(bool check);

		/*!
		 * \brief add a vertex into the class attribute graph
		 * \param name : name of the vertex
		 * \param container : a map which match a vertex with its name
		 */
		void addVertex(const std::string& name, std::map<std::string, VertexDescriptor>& container);

		/*!
		 * \brief add a vertex into a Graph
		 * \param name : name of the vertex
		 * \param container : a map which match a vertex with its name
		 * \param myGraph : a graph
		 * \param positionMap : the positionMap link to the Graph
		 */
		void addVertex(const std::string& name, std::map<std::string, VertexDescriptor>& container, Graph& myGraph, PositionMap& positionMap);

		/*!
		 * \brief add all relational edges into the class attribute graph. Used for RBNToGraph_allAttributconnected 
		 * \param container : a map which match a vertex with its name
		 * \param edgeWeight : weight of relational link
		 */
		void addForeignKeyEdges_multiConnectedAttributs(std::map<std::string, VertexDescriptor>& container, const double edgeWeight);

		/*!
		 * \brief add all relational edges into the class attribute graph. Used for RBNToGraph_ArtificialClassVertex
		 * \param container : a map which match a vertex with its name
		 * \param edgeWeight : weight of relational link
		 */
		void addForeignKeyEdges_artificialClassVertex(std::map<std::string, VertexDescriptor>&, const double edgeWeight);

		/*!
		 * \brief add all relational edges into the class attribute graph. Used for RBNToGraph_PreComputedClassVertex
		 * \param container : a map which match a vertex with its name
		 * \param edgeWeights : a map which contain every relational where the key is a class name
		 */
		void addForeignKeyEdges_artificialClassVertex(std::map<std::string, VertexDescriptor>&, const std::map<std::string, double>& edgeWeights);

		/*!
		 * \brief add all relational edges into the class attribute graph. Used for RBNToGraph_ArtificialClassVertex_NumberOfLink
		 * \param container : a map which match a vertex with its name
		 * \param edgeWeight : the mean relational edge weight
		 * \param probabilisticConnection : a map which contain the computed probabilistic weight. The key is a class name
		 */
		void addForeignKeyEdges_artificialClassVertex_NumberOfLinks(std::map<std::string, VertexDescriptor>&, const double edgeWeight, std::map<std::string, unsigned int>& probabilisticConnection);
		
		/*!
		 *  \brief calculate the thickness of a probabilistic link. For the moment this function returns a random double between 0 and 2 
		 */
		double calculateEdgeThickness();
		
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

		/*!
		 *	\brief add all probabilistic egdes into the class attribute graph
		 *	\param verticeContainer : a map which match a vertex with its name
		 *	\param edgeWweight : the weight of edges
		 */
		void addProbabilistLink(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWweight);
		
		/*!
		 *	\brief add all probabilistic egdes into the class attribute graph
		 *	\param verticeContainer : a map which match a vertex with its name
		 *	\param probWeights : a map which contain computed weight edges, depends on the class name
		 */
		void addProbabilistLink(std::map<std::string, VertexDescriptor>& verticeContainer, const std::map<std::string, double>& probWeights);

		/*!
		 *	\brief add all probabilistic egdes into the class attribute graph, special for RBNToGraph_ArtificialClassVertex_NumberOfLink
		 *	\param verticeContainer : a map which match a vertex with its name
		 *	\param probWeights : a map which contain computed weight edges, depends on the class name
		 */
		void addProbabilistLink_NumberOfLinks(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWweight, std::map<std::string, unsigned int>& probabilisticConnection);

		/*!
		 *	\brief check if an attribut needs to be represented in the PRM
		 *	\param className : the class name of the attribute
		 *	\param attributName : the attribute name
		 *  \return true if the attribute needs to be represented
		 */
		bool checkFkPkAttribute(const std::string& className,const std::string& attributName);
		
		/*!
		 *	\brief get the 2D linear equation of two point
		 *	\param Points : the class name of the attribute
		 *	\param attributName : the attribute name
		 *  \return a pair which contain first a and second b from the ax + b equation
		 */
		std::pair<double, double> get2DLinearEquation(Points, Points);

		/*!
		 *	\brief reverse point coordinates
		 *	\param Points : a point
		 *  \return a point
		 */
		Points& reversePointCoordinate(Points&);

		/*!
		 *	\brief compute probabilistic edge weight with the number of class connection
		 *  \return a map where the key is a class name and the value is the probabilistic edge weight
		 */
		std::map<std::string, unsigned int> computeProbabilisticConnection();

		/*!
		 *	\brief concat 2 class name
		 *	\param name1 : a class name
		 *	\param name2 : a class name
		 *  \return the concatenated string
		 */
		std::string concat2SortedClass(std::string name1, std::string name2);

		/*!
		 *	\brief apply the kamada kamwai algorithm without the circle initialization. Used for the RBNToGraph_PreComputedClassVertex function
		 *	\param sideLenght : the side length of the window
		 */
		void usedKamadaWithoutCircleLayoutInit(const double sideLenght);

		/*!
		 *	\brief compute relational edge weight from the size of classes
		 *	\param classFrom : a class name
		 *	\param classTo : a class name
		 *	\param boundingLength : a map which contain the size of all classes
		 *  \return the relational edge weight
		 */
		double computeEdgeWeight(const std::string& classFrom, const std::string& classTo, const std::map<std::string, double>& boundingLength);

		/*!
		 *	\brief add attributes into the class attribute graph
		 *	\param container : a map which match a vertice with its name
		 *	\param attributeWeight : the weight for all attribute weights
		 *	\param boundingLength : a map which contain the size of all classes
		 */
		void addAttributes(std::map<std::string, VertexDescriptor>& container, const double attributeWeight, std::map<std::string, double> boundingLength);
		
		/*!
		 *	\brief get the class' name from an attribute
		 *	\param vertexName : the name of the attribute
		 *	\return : the class name
		 */
		std::string getVertexClassName(std::string vertexName);

		/*!
		 *	\brief copmute the edge weight from the probabilistic weight mean and the number of connection
		 *	\param linkWeight : the probabilistic weight mean
		 *	\param numberOfLink : the number of probabilistic edges from the class 
		 *	\return : a computed edge
		 */
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
		std::pair<Points, Points> getEdgeCoordinates(boost::graph_traits<Graph>::edge_descriptor&);

		/*!
		 * \brief compute the GCD of two numbers a and b
		 * \return the GCD
		 */
		long pgcd(long a, long b);

		/*!
		 * \brief load only probabilistic edges in a container. Used to compute some aesthetic scores
		 * \param edgesContainer : a vector of edge_descriptor
		 */
		void loadProbabilisticEdges(std::vector<boost::graph_traits<Graph>::edge_descriptor>& edgesContainer);
		
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

		/*!
		 *	\brief transform a PRM into a graph : prototype function
		 *	\param attributeWeight : edge weights for attribute links
		 *	\param FKWeight : edge weights for relational links
		 */
		void RBNToGraph(const double attributeWeight, const double FKWeight);

		/*!
		 *	\brief transform a PRM into a graph : first scenario
		 *	\param attributeWeight : edge weights for attribute links
		 *	\param FKWeight : edge weights for relational links
		 *  \param probWeight : edge weights for probabilistic links
		 */
		void RBNToGraph_AllAttributsConnected(const double attributeWeight, const double FKWeight, const double probWeight);
		
		/*!
		 *	\brief transform a PRM into a graph : second scenario
		 *	\param attributeWeight : edge weights for attribute links
		 *	\param FKWeight : edge weights for relational links
		 *  \param probWeight : edge weights for probabilistic links
		 */
		void RBNToGraph_ArtificialClassVertex(const double attributeWeight, const double FKWeight, const double probWeight);

		/*!
		 *	\brief transform a PRM into a graph : third scenario
		 *	\param attributeWeight : edge weights for attribute links
		 *	\param FKWeight : edge weights for relational links
		 *  \param probWeight : edge weights for probabilistic links
		 */
		void RBNToGraph_ArtificialClassVertex_NumberOfLink(const double attributeWeight, const double FKWeight, const double probWeight);
		
		/*!
		 *	\brief transform a PRM into a graph : fourth scenario
		 *  \param sideLength : side length of the windows
		 *	\param attributeWeight : edge weights for attribute links
		 *  \param probWeight : edge weights for probabilistic links
		 */
		void RBNToGraph_PreComputedClassVertex(const double sideLength, const double attributeWeight, const double probWeight);
		
		/*!
		 *	\brief apply the kamada kawai algorithm on the attribut class graph
		 *	\param sideLength : the sideLenght of the windows
		 */
		void usedKamada(const double sideLenght);

		/************ aesthetic score functions ************/
		
		/*!
		 *	\return the number of probabilistic edges intersection
		 */
		int getNbCrossing();

		/*!
		 *	\return the number of the maximum probabilistic edges intersection
		 */
		int getMaxCrossing();

		/*!
		 *	\return the crossing score
		 */
		double getCrossingScore();

		/*!
		 *	\brief initialize the attribute class graph to test score
		 */
		void initGraph();

		/*!
		 *	\return a pair of extrem points, bounding the PRM
		 */
		std::pair<Points, Points> getExtremGraphPoint(Graph&);

		/*!
		 *	\return the angle score
		 */
		double angleScore();

		/*!
		 *	\return the orthogonality score
		 */
		double orthogonalityScore();

		/*!
		 *	\return the inverse arrows score
		 */
		int inverseArrowScore();
		
		

	};

}

#endif //PRM_DISPLAY_H
