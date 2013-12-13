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
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/kamada_kawai_spring_layout.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <Board.h>

using namespace LibBoard;
typedef boost::square_topology<>::point_type Points;

struct VertexProperties
{
    std::string index;//Classes.Attribut
    Points point;
};

struct EdgeProperty
{
	EdgeProperty(){}
    EdgeProperty(const std::size_t &w):weight(w) {}
    double weight;
};


typedef boost::adjacency_list<boost::vecS,
            boost::vecS, boost::undirectedS,
            VertexProperties, EdgeProperty > Graph;

typedef boost::property_map<Graph, std::string VertexProperties::*>::type VertexIndexPropertyMap;
typedef boost::property_map<Graph, Points VertexProperties::*>::type PositionMap;
typedef boost::property_map<Graph, double EdgeProperty::*>::type WeightPropertyMap;

typedef boost::graph_traits<Graph>::vertex_descriptor VirtexDescriptor;


typedef struct ColorClass {
	int red; 
	int blue; 
	int green;
	std::string classeName;
};

namespace prm{

	class PRMDisplay{
	private: 
		Graph graph;
		boost::shared_ptr<prm::RBN> rbn;
		PositionMap  positionMap;
		Board board;
		VertexIndexPropertyMap vertexIdPropertyMap;
		std::vector<ColorClass> listColor;
	public:
		
		/*!
		 * \brief Ctor.
		 * \param schema : the RelationalSchema giving the domain and the structure of the problem to modelize
		 */
		PRMDisplay();
		PRMDisplay(const boost::shared_ptr<prm::RBN> rbn);
		PRMDisplay(const boost::shared_ptr<prm::RBN> rbn, const PositionMap& postionMap, const VertexIndexPropertyMap& vertexNamemap,const Graph& graph);
		PRMDisplay(const PositionMap& postionMap, const VertexIndexPropertyMap& vertexNamemap, const Graph& graph);
		/*!
		 * \brief Dtor.
		 */
		 ~PRMDisplay();
		void placeVertex();
		void placeRelationnalLink();
		void placeProbabilistLink();
		void display(const std::string& path, const std::string& name);

	};

}

#endif //PRM_DISPLAY_H