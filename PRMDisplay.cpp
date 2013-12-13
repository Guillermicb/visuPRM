
#include "PRMDisplay.h";
#include "RBN.h";
#include <Shapes.h>

using namespace LibBoard;
using namespace prm;

PRMDisplay::PRMDisplay(const boost::shared_ptr<prm::RBN> rbn):rbn(rbn), positionMap(){
	board.clear( Color(255,255,255) );
	board.setUnit(1, Board::UCentimeter);
	 board << LibBoard::Rectangle( 0, 0, 500, 500, Color::Black, Color::None, 0.1 );
  board.setPenColorRGBi( 255, 0, 0);
  board.setLineWidth( 2 );
  srand (time(NULL));
}
PRMDisplay::PRMDisplay(const boost::shared_ptr<prm::RBN> rbn, const PositionMap& postionMap, const VertexIndexPropertyMap& vertexNamemap, const Graph& graph):rbn(rbn), positionMap(postionMap), vertexIdPropertyMap(vertexNamemap), graph(graph){
	this->board.clear( Color(255,255,255) );
	board.setUnit(1, Board::UCentimeter);
  board.setPenColorRGBi( 255, 0, 0);
  board << LibBoard::Rectangle( 0, 0, 500, 500, Color::Black, Color::None, 0.1 );
  board.setLineWidth( 2 );
  srand (time(NULL));
}

PRMDisplay::PRMDisplay(const PositionMap& postionMap, const VertexIndexPropertyMap& vertexNamemap, const Graph& graph):positionMap(postionMap), vertexIdPropertyMap(vertexNamemap), graph(graph){
	this->board.clear( Color(255,255,255) );
	board.setUnit(1, Board::UCentimeter);
  board.setPenColorRGBi( 255, 0, 0);
  board.setLineWidth( 2 );
   board << LibBoard::Rectangle( 0, 0, 500, 500, Color::Black, Color::None, 0.1 );
  srand (time(NULL));
}

void PRMDisplay::placeVertex(){
	std::map<std::string, Color> colorclasse; 
	int nbclasse=5;
	 srand ( time(NULL) );
	boost::graph_traits<Graph>::vertex_iterator i, end;
    for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
        std::cout << "ID: (" << vertexIdPropertyMap[*i] << ") x: " << positionMap[*i][0] << " y: " << positionMap[*i][1] << "\n";
		std::string classename=vertexIdPropertyMap[*i].substr(0,vertexIdPropertyMap[*i].find("."));
		bool trouve=false;
		if (listColor.empty()){
			ColorClass c;
			c.classeName=classename;
				c.red=rand() % 255;
				c.blue=rand() % 255;
				c.green=rand() % 255;
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
						//std::cout<<" second "<< it->first<< " coordonées "<< positionMap[*i1][0]<<" "<<positionMap[*i1][1] <<std::endl;
						//std::cout<<" " <<positionMap[*i][0]<<" "<<positionMap[*i][1]<<" "<< positionMap[*i1][0]<<" "<<positionMap[*i1][1]<<" "<<std::endl;
							float x1,x2,y1,y2;
							x1=positionMap[*i][0];
							y1=positionMap[*i][1];
							x2=positionMap[*i1][0];
							y2=positionMap[*i1][1];
							// Comparer les position pour placer x et Y correctement
							if (positionMap[*i][1]<positionMap[*i1][1]){// le noeud 1 est plus haut que noeud 2
								y1+=.5;
							}else {
								y2+=.5;
							}
							if (positionMap[*i][0]<positionMap[*i1][0]){// le noeud 1 est plus droite que noeud 2
								x1+=3;
							}else {
								x2+=3;
							}
							board.drawLine(x1,y1,x2,y2);
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
			std::cout<< "son 2: "<< vertexIdPropertyMap[*i]<<std::endl;
			if (seq.dim()>0) {
				std::cout<< "pas vide"<<std::endl;
				for(unsigned int j = 0; j < seq.dim(); j++){
					std::cout<< "size: "<<seq.dim()<<" parents " <<dynamic_pointer_cast<IRBNSimpleVariable>(seq[j])->getBaseName()<<std::endl;
					boost::graph_traits<Graph>::vertex_iterator i1, end1;
					for (boost::tie(i1, end1) = boost::vertices(graph); i1 != end1; ++i1) {
						if (vertexIdPropertyMap[*i1].compare(dynamic_pointer_cast<IRBNSimpleVariable>(seq[j])->getBaseName())==0){
							float x1,x2,y1,y2;
							x1=positionMap[*i1][0];
							y1=positionMap[*i1][1];
							x2=positionMap[*i][0];
							y2=positionMap[*i][1];
							// Comparer les position pour placer x et Y correctement
							if (positionMap[*i1][1]<positionMap[*i][1]){// le noeud 1 est plus haut que noeud 2
								y1+=.5;
							}else {
								y2+=.5;
							}
							if (positionMap[*i1][0]<positionMap[*i][0]){// le noeud 1 est plus droite que noeud 2
								x1+=3;
							}else {
								x2+=3;
							}
							board.drawArrow(x1,y1, x2,y2);
							//std::cout<<" " <<positionMap[*i1][0]<<" "<<positionMap[*i1][1]<<" "<< positionMap[*i][0]<<" "<<positionMap[*i][1]<<" "<<std::endl;
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
	board.saveSVG(completepath.append(".svg").c_str(),500,500);
	//board.saveFIG(completepath.append(".fig").c_str(),Board::BoundingBox, 10.0);
}