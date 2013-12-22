
#include "PRMDisplay.h"
#include "RBN.h"
#include <Shapes.h>
#include <sstream>

using namespace LibBoard;
using namespace prm;

PRMDisplay::PRMDisplay(const boost::shared_ptr<prm::RBN> rbn, const Graph& graph):rbn(rbn), graph(graph){
	board.clear( Color(255,255,255) );
	board.setUnit(1, Board::UCentimeter);
	board.setPenColorRGBi( 255, 0, 0);
	board.setLineWidth( 1 );
	srand (time(NULL));
	
}
PRMDisplay::PRMDisplay(const boost::shared_ptr<prm::RBN> rbn, const PositionMap& postionMap, const VertexIndexPropertyMap& vertexNamemap, const Graph& graph):rbn(rbn), positionMap(postionMap), vertexIdPropertyMap(vertexNamemap), graph(graph){
	this->board.clear( Color(255,255,255) );
	board.setUnit(1, Board::UCentimeter);
  board.setPenColorRGBi( 255, 0, 0);
  board << LibBoard::Rectangle( -8, 12, 16, 24, Color::Black, Color::None, 0.1 );
  board.setLineWidth( 2 );
  srand (time(NULL));
}

PRMDisplay::PRMDisplay(const PositionMap& postionMap, const VertexIndexPropertyMap& vertexNamemap, const Graph& graph):positionMap(postionMap), vertexIdPropertyMap(vertexNamemap), graph(graph){
	this->board.clear( Color(255,255,255) );
	board.setUnit(1, Board::UCentimeter);
  board.setPenColorRGBi( 255, 0, 0);
  board.setLineWidth( 2 );
  srand (time(NULL));
}

float  random_gray() {
  return rand() / 255;
}

void PRMDisplay::placeVertex(){
	std::map<std::string, Color> colorclasse; 
	int nbclasse=5;
	srand( static_cast<unsigned int>( time(0) ) );
	boost::graph_traits<Graph>::vertex_iterator i, end;
    for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
        std::cout << "ID: (" << vertexIdPropertyMap[*i] << ") x: " << positionMap[*i][0] << " y: " << positionMap[*i][1] << "\n";
		std::string classename=vertexIdPropertyMap[*i].substr(0,vertexIdPropertyMap[*i].find("."));
		bool trouve=false;
		if (listColor.empty()){
			ColorClass c;
			c.classeName=classename;
				c.red=rand() / 255;
				c.blue=rand() / 255;
				c.green=rand() / 255;
			listColor.push_back(c);
			board.setPenColorRGBi(c.red, c.green, c.blue);
		}else{

			
			for (std::vector<ColorClass>::iterator it = listColor.begin();it != listColor.end(); ++it){
				std::cout<<"\t"<<it->classeName<<std::endl;
				if (it->classeName.compare(classename)==0){
				 trouve=true;
				 board.setPenColorRGBi(it->red, it->green, it->blue);
				 break;
				}
			}
			std::cout<<"\t"<<"fin du while"<<std::endl;
			if(!trouve){
				ColorClass c;
				c.classeName=classename;
				c.red=rand() / 255;
				c.blue=rand() / 255;
				c.green=rand() / 255;
				listColor.push_back(c);
				board.setPenColorRGBi(c.red, c.green, c.blue);
			}
		}
		/*
		if (!colorclasse.empty()){
			std::map<std::string,Color>::iterator it1=colorclasse.find(classename);
			if (it1==colorclasse.end()){
				Color c=  new Color(rand() / 255, rand() / 255, rand() / 255);
				board.setPenColorRGBi(rand() / 255, rand() / 255, rand() / 255 );
				colorclasse.insert(std::pair<std::string, Color>(classename , c));
			}
		}else{
			Color c=  new Color(rand() / 255, rand() / 255, rand() / 255);
			board.setPenColorRGBi(rand() / 255, rand() / 255, rand() / 255);
			colorclasse.insert(std::pair<std::string, Color>(classename , c));
			
		}
	*/
		board.drawRectangle(positionMap[*i][0],positionMap[*i][1], 3,.5);
		board.drawText(positionMap[*i][0]+.1,positionMap[*i][1]+0.4, vertexIdPropertyMap[*i] );
		
    }
	

}
void PRMDisplay::placeRelationnalLink(){
	board.setPenColorRGBi( 255, 0, 0);
	std::cout<<"Place Relationnal Link"<<std::endl;
	std::map<std::string, std::string> result; // model (classe.FK, classe1.PK)
	RefSlotsMultimap refSlots = rbn->getSchema().getRefSlots();
	// Fisrt: get all the link between PK and FK	
	for(RefSlotsMultimap::iterator rb = refSlots.begin(); rb != refSlots.end(); ++rb){
		std::cout << "\t\t<rbn-ClassesLink from='" << rb->first << "' to='" << rb->second.second->getName() << "' fk='" << rb->second.first.getName() << "'/>" << std::endl;
		std::string classPK=rb->second.second->getName().c_str();
		std::string attPK= rbn->getSchema().getClass(classPK).getPK().front();
		classPK.append(".");
		classPK.append(attPK);
		//std::cout << "\t primaryKey "<<classPK;
		std::string classFK=rb->first.c_str();;
		classFK.append(".");
		classFK.append(rb->second.first.getName());
		//std::cout << "\t foriegnKey "<<classPK;
		result.insert(std::pair<std::string, std::string>(classFK, classPK));
	}
	// Second: display the links
	boost::graph_traits<Graph>::vertex_iterator i, end;
	boost::graph_traits<Graph>::vertex_iterator i1, end1;
	for (std::map<std::string,std::string>::iterator it=result.begin(); it!=result.end(); ++it){
		std::cout<<" first "<< it->first<< "second "<< it->second <<std::endl;

		for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
			if (vertexIdPropertyMap[*i].compare(it->first)==0) {
				std::cout<<" first "<< it->first<< " coordonées "<< positionMap[*i][0]<<" "<<positionMap[*i][1] <<std::endl;
				for (boost::tie(i1, end1) = boost::vertices(graph); i1 != end1; ++i1) {
					if (vertexIdPropertyMap[*i1].compare(it->second)==0) {
						std::cout<<" second "<< it->first<< " coordonées "<< positionMap[*i1][0]<<" "<<positionMap[*i1][1] <<std::endl;
						std::cout<<" " <<positionMap[*i][0]<<" "<<positionMap[*i][1]<<" "<< positionMap[*i1][0]<<" "<<positionMap[*i1][1]<<" "<<std::endl;
						// A FAIRE:comparer les position pour placer x et Y correctement
						board.drawLine(positionMap[*i][0],positionMap[*i][1]+0.4, positionMap[*i1][0],positionMap[*i1][1]);
						break;
					}
				}
				break;
			}
		}
	}
				
}
void PRMDisplay::placeProbabilistLink(){ 
	board.setPenColorRGBi( 0, 0,255);
	std::cout<<"Place Probabilist Link"<<std::endl;
	boost::graph_traits<Graph>::vertex_iterator i, end;
	RBNVariablesSequence seq;
	 for (boost::tie(i, end) = boost::vertices(graph); i != end; i++) {
		
		if (rbn->existsNode(vertexIdPropertyMap[*i])){
			seq=rbn->getParents(vertexIdPropertyMap[*i]);
			//std::cout<< "son 2: "<< vertexIdPropertyMap[*i]<<std::endl;
			if (seq.dim()>0) {
				//std::cout<< "pas vide"<<std::endl;
				for(unsigned int j = 0; j < seq.dim(); j++){
					//std::cout<< "size: "<<seq.dim()<<" parents " <<dynamic_pointer_cast<IRBNSimpleVariable>(seq[j])->getBaseName()<<std::endl;
					boost::graph_traits<Graph>::vertex_iterator i1, end1;
					for (boost::tie(i1, end1) = boost::vertices(graph); i1 != end1; ++i1) {
						if (vertexIdPropertyMap[*i1].compare(dynamic_pointer_cast<IRBNSimpleVariable>(seq[j])->getBaseName())==0){
						
							// comparer les position pour placer x et Y correctement
							board.drawArrow(positionMap[*i1][0],positionMap[*i1][1], positionMap[*i][0],positionMap[*i][1]);
							std::cout<<" " <<positionMap[*i1][0]<<" "<<positionMap[*i1][1]<<" "<< positionMap[*i][0]<<" "<<positionMap[*i][1]<<" "<<std::endl;
							break;
						}
					}
				}
			}
		}
	 }
			
	
}
void PRMDisplay::display(const std::string& path, const std::string& name){
	std::string completepath;
	completepath.append(path);
	completepath.append("\\");
	completepath.append(name);
	//completepath.append(".svg");
	board.saveSVG(completepath.append(".svg").c_str(),4000,4000);
	//board.saveFIG(completepath.append(".fig").c_str(),Board::BoundingBox, 10.0);
}

void PRMDisplay::RBNToGraph(const double attributeWeight, const double classWeight, const double FKWeight) {
	Class aClass;
	VertexDescriptor vd;
	std::map<std::string, VertexDescriptor> container;
	positionMap = boost::get(&VertexProperties::point, graph);
	vertexIdPropertyMap = boost::get(&VertexProperties::index, graph);
	WeightPropertyMap weightPropertyMap = boost::get(&EdgeProperty::weight, graph);
	RelationalSchema schema = rbn->getSchema();
	std::vector<std::string> classnames = schema.getClassNames();
	for(std::vector<std::string>::iterator it = classnames.begin(); it != classnames.end(); ++it)
	{
		std::string classname = *it;

		aClass = schema.getClass(classname);
		std::vector<std::string> attributesName = aClass.getAttributeNames();
		//std::vector<std::string>::iterator it2 = attributesName.begin();

		//pour chaque attribut, ajout d'un nouveau sommet aux coordonnée (1,1)
		for(std::vector<std::string>::iterator it1 = attributesName.begin(); it1!=attributesName.end(); it1++)
		{
			std::stringstream ss;
			ss << classname << "." << *it1;

			vd = boost::add_vertex(graph);

			vertexIdPropertyMap[vd] = ss.str();
			container[ss.str()] = vd;
			positionMap[vd][0]=1;
			positionMap[vd][1]=1;
			// si l'itérateur est différent de begin, alors on itere pour ajouter une arête jusqu'à l'itérateur it1 actuel
			if(attributesName.begin() != it1)
			{
				//Ajout d'arête entre le nouveau sommet et ceux du graphe qui appartiennent à la même classe. Le poid est de 2
				for(std::vector<std::string>::iterator it3 = attributesName.begin(); it3 != it1; it3++)
				{
					std::stringstream ss2;
					ss2 << classname << "." << *it3;
					boost::add_edge(container[ss.str()], container[ss2.str()], EdgeProperty(attributeWeight), graph);
				}
			}
		} 
		/***** ajout d'arête entre toutes les classes *****/
		/*if( it != classnames.begin()) {
			for(std::vector<std::string>::iterator classIterator = classnames.begin() ; classIterator != it ; ++classIterator)
			{
				std::string PK1 = *it;
				std::string PK2 = *classIterator;
				PK1.append(".");
				PK2.append(".");
				PK1.append(schema.getClass(*it).getPK().front());
				PK2.append(schema.getClass(*classIterator).getPK().front());
				boost::add_edge(container[PK1], container[PK2], EdgeProperty(classWeight), graph);
			}
		}*/
	}


	
	
	
	/***** ajout d'arête pour chaque clé étrangère ****/
	RefSlotsMultimap foreignKeys = schema.getRefSlots();
	std::string attPK;
	std::string classPK;
	std::string classFK;
	for(RefSlotsMultimap::iterator rb = foreignKeys.begin(); rb != foreignKeys.end(); ++rb){
		classPK=rb->second.second->getName().c_str();
		attPK= rbn->getSchema().getClass(classPK).getPK().front();
		classPK.append(".");
		classPK.append(attPK);
		//std::cout << "\t primaryKey "<<classPK;
		classFK=rb->first.c_str();;
		classFK.append(".");
		classFK.append(rb->second.first.getName());
		//std::cout << "\t foriegnKey "<<classFK;
		boost::add_edge(container[classFK], container[classPK], EdgeProperty(FKWeight), graph);
	}
}

void PRMDisplay::usedKamada(const double sideLenght){
	typedef square_topology<> Topology;
	minstd_rand gen;
	Topology topology(gen, 50.0);
	Topology::point_type origin;
	origin[0] = origin[1] = 50.0;
	Topology::point_difference_type extent;
	extent[0] = extent[1] = 50.0;
	WeightPropertyMap weightPropertyMap = boost::get(&EdgeProperty::weight, graph);
	boost::circle_graph_layout(graph, positionMap, sideLenght);
	//boost::random_graph_layout(graph, positionMap, topology);
	bool result;
	result=boost::kamada_kawai_spring_layout(graph, 
                                       positionMap,
                                       weightPropertyMap,topology,
                                       side_length(sideLenght),
                                       kamada_kawai_done());
	if (!result){
		std::cout << "kamada_kawai_spring_layout returned false\n "<<std::endl;
	}else{
		std::cout << "now displaying"<<std::endl;
	}
	boost::graph_traits<Graph>::vertex_iterator i, end;
	for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
		positionMap[*i][0]=positionMap[*i][0]+sideLenght;
		positionMap[*i][1]=positionMap[*i][1]+sideLenght;
	}

	//print_graph_layout(graph, positionMap, topology);
}


/*void print_graph_layout(const Graph& g, PositionMap position, const Topology& topology)
{
  typedef typename Topology::point_type Point;
  // Find min/max ranges
  Point min_point = position[*vertices(g).first], max_point = min_point;
  BGL_FORALL_VERTICES_T(v, g, Graph) {
    min_point = topology.pointwise_min(min_point, position[v]);
    max_point = topology.pointwise_max(max_point, position[v]);
  }

  for (int y = (int)min_point[1]; y <= (int)max_point[1]; ++y) {
    for (int x = (int)min_point[0]; x <= (int)max_point[0]; ++x) {
      typename graph_traits<Graph>::vertex_iterator vi, vi_end;
      // Find vertex at this position
      typename graph_traits<Graph>::vertices_size_type index = 0;
      for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi, ++index) {
        if ((int)position[*vi][0] == x && (int)position[*vi][1] == y)
          break;
      }

      if (vi == vi_end) std::cout << ' ';
      else std::cout << (char)(index + 'A');
    }
    std::cout << std::endl;
  }
}*/
