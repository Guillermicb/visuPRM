
#include "PRMDisplay.h"
#include "RBN.h"
#include <Shapes.h>
#include <sstream>


using namespace LibBoard;
using namespace prm;

PRMDisplay::PRMDisplay(const boost::shared_ptr<prm::RBN> rbn, const Graph& graph):rbn(rbn), graph(graph){
	board.clear( Color(255,255,255) );
	board.setUnit(8, Board::UPoint);
	board.setFontSize(10);
	board.setPenColorRGBi( 255, 0, 0);
	board.setLineWidth(0.5);
	srand (time(NULL));
	delta=1.0;
	
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
		std::string attribute=vertexIdPropertyMap[*i].substr(vertexIdPropertyMap[*i].find(".")+1, vertexIdPropertyMap[*i].length());
		// couleur
		makeVertexColor(classename);
		lengthmap[*i]=attribute.length()/1.35;
		if (vertexIdPropertyMap[*i].find(".")!=std::string::npos) {
			//if (checkFkPkAttribute(classename, attribute)){
					board.drawText(positionMap[*i][0]+.5,positionMap[*i][1]+1, vertexIdPropertyMap[*i].substr(vertexIdPropertyMap[*i].find(".")+1, vertexIdPropertyMap[*i].length()) );
					board.drawEllipse(positionMap[*i][0]+lengthmap[*i]/2, positionMap[*i][1]+0.75, lengthmap[*i]/2	, 1 );
					//board.drawRectangle(positionMap[*i][0],positionMap[*i][1],lengthmap[*i],1.5 );
			//}
		}
	}
	placeClasse();

}

bool PRMDisplay::checkFkPkAttribute(const std::string& classename,const std::string& attribute){
	if (!rbn->getSchema().isFKAttribute(classename, attribute)){
				if (!rbn->getSchema().getClass(classename).isPK(attribute)) {
					return true; 
				}
	} 
	return false;
}

void PRMDisplay::makeVertexColor(const std::string& classename){
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
				if (it->classeName.compare(classename)==0){
				 trouve=true;
				 board.setPenColorRGBi(it->red, it->green, it->blue);
				 break;
				}
			}
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
}

void PRMDisplay::placeClasse(){
	boost::graph_traits<Graph>::vertex_iterator i, end;
    for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
		std::string classename=vertexIdPropertyMap[*i].substr(0,vertexIdPropertyMap[*i].find("."));
		std::string attribute=vertexIdPropertyMap[*i].substr(vertexIdPropertyMap[*i].find(".")+1, vertexIdPropertyMap[*i].length());
		if (vertexIdPropertyMap[*i].find(".")!=std::string::npos) {
		
			bool trouve=false;
			if (listRect.empty()){
				RectClass c;
				c.classeName=classename;
				c.x=positionMap[*i][0];
				c.y=positionMap[*i][1];
				c.len=lengthmap[*i];
				c.hei=1.5;
			
				listRect.push_back(c);
			
			}else{		
				for (std::vector<RectClass>::iterator it = listRect.begin();it != listRect.end(); ++it){
					if (it->classeName.compare(classename)==0){
						trouve=true;
						// je regle la longueur du rectangle
						if ((it->x<=positionMap[*i][0]) && ((it->x)+(it->len)>=(positionMap[*i][0]+lengthmap[*i]))) {

						}else if (it->x>positionMap[*i][0]){
							it->len+=std::abs(it->x-positionMap[*i][0]);
						}else if ((it->x)+(it->len)<(positionMap[*i][0]+lengthmap[*i])){
							it->len+=std::abs(positionMap[*i][0]+lengthmap[*i]-(it->x+it->len));
						}
						// je regle la hauteur du rectangle
						if ((it->y<=positionMap[*i][1]) && ((it->y)+(it->hei)>=(positionMap[*i][1]+1.5))) {

						}else if (it->y>positionMap[*i][1]){
							it->hei+=std::abs(it->y-positionMap[*i][1]);
						}else if ((it->y)+(it->hei)<(positionMap[*i][1]+1.5)){
							it->hei+=std::abs(positionMap[*i][1]+1.5-(it->y+it->hei));
						}
						//je regle le point en haut à gauche du rectangle
						if (it->x>positionMap[*i][0]) { it->x=positionMap[*i][0];}
						if (it->y>positionMap[*i][1]) { it->y=positionMap[*i][1];}
					
					 break;
					}
				}
			
				if(!trouve){
					std::cout<<"Classe: "<<classename<<std::endl;
					RectClass c;
					c.classeName=classename;
					c.x=positionMap[*i][0];
					c.y=positionMap[*i][1];
					c.len=lengthmap[*i];
					c.hei=1.5;
				
					listRect.push_back(c);
				}
			}
		
		}
	}
	drawClass();
}

void PRMDisplay::drawClass(){
	// je dessine les rectangles de classe avec la bonne couleur de pinceau
	for (std::vector<RectClass>::iterator it = listRect.begin();it != listRect.end(); ++it){
		for (std::vector<ColorClass>::iterator it1 = listColor.begin();it1 != listColor.end(); ++it1){
				if (it1->classeName.compare(it->classeName)==0){
				 board.setPenColorRGBi(it1->red, it1->green, it1->blue);
				 break;
				}
			}
		
		board.drawRectangle(it->x-delta,it->y-delta-1.5,it->len+delta+delta,it->hei+delta+delta+1.5);	
		
		board.drawText(it->x-delta+.1,it->y-delta-0.2, it->classeName,10 );
		it->y+=-1.5-delta; it->hei+=+delta+delta+1.5;it->x-=delta;it->len+=delta+delta;
	}
}

void PRMDisplay::placeRelationnalLink(){
	board.setPenColorRGBi( 255, 0, 0);
	
	std::map<std::string, std::string> result; // model (classe.FK, classe1.PK)
	RefSlotsMultimap refSlots = rbn->getSchema().getRefSlots();
	// Fisrt: get all the link between PK and FK	
	for(RefSlotsMultimap::iterator rb = refSlots.begin(); rb != refSlots.end(); ++rb){
		std::string classPK=rb->second.second->getName().c_str();
		std::string classFK=rb->first.c_str();
		float x1,x2,y1,y2;
		 RectClass PK, FK;
		for (std::vector<RectClass>::iterator it5 = listRect.begin();it5 != listRect.end(); ++it5){
			
			if (classPK.compare(it5->classeName)==0) {
				x1=it5->x;
				y1=it5->y;
				PK.classeName=it5->classeName;
				PK.x=it5->x;
				PK.y=it5->y;
				PK.hei=it5->hei;
				PK.len=it5->len;
			}
			if (classFK.compare(it5->classeName)==0) {
				x2=it5->x;
				y2=it5->y;
				FK.classeName=it5->classeName;
				FK.x=it5->x;
				FK.y=it5->y;
				FK.hei=it5->hei;
				FK.len=it5->len;
			}
		}
		std::cout<<"("<<x1<<","<<y1<<")"<<"("<<x2<<","<<y2<<")"<<std::endl;
		
		if (x1+PK.len<x2+FK.len/2) {// le rectangle 2 est a droite du rectangle 1  
			if ((y1>y2) && (y1<y2+FK.hei)){
				x1+=PK.len; y1+=PK.hei/2;y2+=FK.hei/2;
				drawRelationnalLink(2,x1,y1,x2,y2 );
			}else if ((y1<y2)) {// le rectangle 2 est en dessous du rectangle 1 
				x1+=PK.len; y1+=PK.hei/2; x2+=FK.len/2;
				drawRelationnalLink(1,x1,y1,x2,y2 );
			}else{// le rectangle 2 est au dessus du rectangle 1 
				x1+=PK.len; y1+=PK.hei/2; x2+=FK.len/2;y2+=FK.hei;
				drawRelationnalLink(1,x1,y1,x2,y2 );
			}
			
		}else{// le rectangle 1 est a droite du rectangle 2  
			if ((y1>y2) && (y1<y2+FK.hei)){
				 y1+=PK.hei/2;y2+=FK.hei/2;x2+=FK.len;
				 drawRelationnalLink(2,x1,y1,x2,y2 );
			}else if ((y1<y2)) {// le rectangle 2 est en dessous du rectangle 1 
				 y1+=PK.hei/2; x2+=FK.len/2;
				 drawRelationnalLink(1,x1,y1,x2,y2 );
			}else{// le rectangle 2 est au dessus du rectangle 1
				y1+=PK.hei/2; x2+=FK.len/2;y2+=FK.hei;
				drawRelationnalLink(1,x1,y1,x2,y2 );
			}
		}

		
		
		
		//result.insert(std::pair<std::string, std::string>(classFK, classPK));
	}
}

void PRMDisplay::drawRelationnalLink(int nbCoude, float x1,float y1,float x2,float y2 ){
	float tmpX, tempY;
	switch (nbCoude){
	case 1: 
		board.drawLine(x1,y1,x2,y1);
		board.drawLine(x2,y1,x2,y2);
		break; 
	case 2: 
		tmpX=std::abs(x1-x2)/2;
		if (x1<x2){tmpX+=x1;} else {tmpX+=x2;}
		board.drawLine(x1,y1,tmpX,y1);
		board.drawLine(tmpX,y1,tmpX,y2);
		board.drawLine(tmpX,y2,x2,y2);

		break;
	}
}

double PRMDisplay::distanceBetweenDot(float x1,float y1,float x2,float y2 ){
	return std::sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}


double PRMDisplay::minDistance4point(float a,float b,float c,float d ){
	float mini; 
	mini=min(a,b);
	mini=min(mini,c);
	mini=min(mini,d);
	return mini;
}

void PRMDisplay::placeProbabilistLink(){ 
	board.setPenColorRGBi( 0, 0,255);
	boost::graph_traits<Graph>::vertex_iterator i, end;
	RBNVariablesSequence seq;
	 for (boost::tie(i, end) = boost::vertices(graph); i != end; i++) {
		
		if (rbn->existsNode(vertexIdPropertyMap[*i])){
			seq=rbn->getParents(vertexIdPropertyMap[*i]);
			if (seq.dim()>0) {
				for(unsigned int j = 0; j < seq.dim(); j++){
					boost::graph_traits<Graph>::vertex_iterator i1, end1;
					for (boost::tie(i1, end1) = boost::vertices(graph); i1 != end1; ++i1) {
						if (vertexIdPropertyMap[*i1].compare(dynamic_pointer_cast<IRBNSimpleVariable>(seq[j])->getBaseName())==0){
							float x1,x2,y1,y2, dist1, dist2, dist3, dist4;
							x1=positionMap[*i1][0];
							y1=positionMap[*i1][1];
							x2=positionMap[*i][0];
							y2=positionMap[*i][1];
							// Comparer les position pour placer x et Y correctement
							if ((x1>x2)&&(y1<y2)){
								dist1=distanceBetweenDot(x1,y1+0.75,x2+lengthmap[*i]/2,y2);
								dist2=distanceBetweenDot(x1,y1+0.75,x2+lengthmap[*i],y2+0.75);
								dist3=distanceBetweenDot(x1+lengthmap[*i1]/2,y1+1.5,x2+lengthmap[*i]/2,y2);
								dist4=distanceBetweenDot(x1+lengthmap[*i1]/2,y1+1.5,x2+lengthmap[*i],y2+0.75);
								float distmin=minDistance4point(dist1,dist2,dist3,dist4);
								if (distmin==dist1){
									y1+=0.75;
									x2+=lengthmap[*i]/2;
									y2-=0.2;
								}else if (distmin==dist2){
									y1+=0.75;
									x2+=lengthmap[*i];
									y2+=0.75;
								}else if (distmin==dist3){
									x1+=lengthmap[*i1]/2;
									y1+=1.7;
									x2+=lengthmap[*i]/2;
									y2-=0.2;
								}else if (distmin==dist4){
									x1+=lengthmap[*i1]/2;y1+=1.7;x2=+lengthmap[*i];y2+=0.75;
								}

							}else if ((x1>x2)&&(y1>y2)){
								dist1=distanceBetweenDot(x1,y1+0.75,x2+lengthmap[*i]/2,y2+1.5);
								dist2=distanceBetweenDot(x1,y1+0.75,x2+lengthmap[*i],y2+0.75);
								dist3=distanceBetweenDot(x1+lengthmap[*i1]/2,y1+1.5,x2+lengthmap[*i]/2,y2+1.5);
								dist4=distanceBetweenDot(x1+lengthmap[*i1]/2,y1+1.5,x2+lengthmap[*i],y2+0.75);
								float distmin=minDistance4point(dist1,dist2,dist3,dist4);
								if (distmin==dist1){
									y1+=0.75;
									y2+=1.7;
									x2+=lengthmap[*i]/2;
								}else if (distmin==dist2){
									y1+=0.75;
									x2+=lengthmap[*i];
									y2+=0.75;
								}else if (distmin==dist3){
									x1+=lengthmap[*i1]/2;
									y1+=1.7;
									y2+=1.7;
									x2+=lengthmap[*i]/2;
								}else if (distmin==dist4){
									x1+=lengthmap[*i1]/2;
									y1+=1.7;
									x2=+lengthmap[*i];
									y2+=0.75;
								}
							}else if ((x1<x2)&&(y1>y2)){
								dist1=distanceBetweenDot(x1+lengthmap[*i1]/2,y1,x2,y2+0.75);
								dist2=distanceBetweenDot(x1+lengthmap[*i1]/2,y1,x2+lengthmap[*i]/2,y2+1.5);
								dist3=distanceBetweenDot(x1+lengthmap[*i1],y1+0.75,x2,y2+0.75);
								dist4=distanceBetweenDot(x1+lengthmap[*i1],y1+0.75,x2+lengthmap[*i]/2,y2+1.5);
								float distmin=minDistance4point(dist1,dist2,dist3,dist4);
								if (distmin==dist1){
									x1+=lengthmap[*i1]/2;y2+=0.75;y1-=0.2;
								}else if (distmin==dist2){
									x1+=lengthmap[*i1]/2;x2+=lengthmap[*i]/2;y2+=1.7;y1-=0.2;
								}else if (distmin==dist3){
									x1+=lengthmap[*i1];y1+=0.75;y2+=0.75;
								}else if (distmin==dist4){
									x1+=lengthmap[*i1];y1=+0.75;x2+=lengthmap[*i]/2;y2+=1.7;
								}
							}else if ((x1<x2)&&(y1<y2)){
								dist1=distanceBetweenDot(x1+lengthmap[*i1]/2,y1+0.75,x2+lengthmap[*i]/2,y2);
								dist2=distanceBetweenDot(x1+lengthmap[*i1]/2,y1+0.75,x2,y2+0.75);
								dist3=distanceBetweenDot(x1+lengthmap[*i1],y1+0.75,x2+lengthmap[*i]/2,y2);
								dist4=distanceBetweenDot(x1+lengthmap[*i1],y1+0.75,x2,y2+0.75);
								float distmin=minDistance4point(dist1,dist2,dist3,dist4);
								if (distmin==dist1){
									x1+=lengthmap[*i1]/2;y1+=0.75;x2+=lengthmap[*i]/2;y2-=0.2;
								}else if (distmin==dist2){
									x1+=lengthmap[*i1]/2;y1+=0.75;y2+=0.75;
								}else if (distmin==dist3){
									x1+=lengthmap[*i1];
									y1+=0.75;x2+=lengthmap[*i]/2;
									y2-=0.2;
								}else if (distmin==dist4){
									x1+=lengthmap[*i1];y1+=0.75;y2+=0.75;
								}
							}
							board.drawArrow(x1,y1, x2,y2);
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

void PRMDisplay::RBNToGraph(const double attributeWeight, const double FKWeight) {
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

void PRMDisplay::usedKamada(const double sidelength){
	boost::minstd_rand gen;
	Topology topology(gen, sidelength);

	WeightPropertyMap weightPropertyMap = boost::get(&EdgeProperty::weight, graph);
	boost::circle_graph_layout(graph, positionMap, sidelength);
	
	bool result = boost::kamada_kawai_spring_layout(graph, 
                                       positionMap,
                                       weightPropertyMap,
									   topology,
                                       side_length(sidelength),
									   boost::layout_tolerance<>());
	
	displayKamadaCheck(result);
	adjustDisplayAfterKamada(sidelength);

}

void PRMDisplay::adjustDisplayAfterKamada(const double length){
	boost::graph_traits<Graph>::vertex_iterator i, end;
	Topology topology;
	Points min_point = positionMap[*vertices(graph).first], max_point = min_point;
	const int margin = 5;

	for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
		min_point = topology.pointwise_min(min_point, positionMap[*i]);
		//max_point = topology.pointwise_max(max_point, positionMap[*i]);
	}

	for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
		positionMap[*i][0] += margin - min_point[0];
		positionMap[*i][1] += margin - min_point[1];
	}
	/*for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
		positionMap[*i][0] = positionMap[*i][0]/8;
		positionMap[*i][1] = positionMap[*i][1]/8;
	}*/
 

}

void PRMDisplay::displayKamadaCheck(bool kamadaResult){
	if (kamadaResult)
		std::cout << "now displaying"<<std::endl;
	else
		std::cerr << "kamada_kawai_spring_layout failed" << std::endl;
}

void PRMDisplay::RBNToGraph_AllAttributsConnected(const double attributeWeight, const double FKWeight, const double probWeight){
	/*rechercher toutes classes et tous les attributs*/
	std::map<std::string, VertexDescriptor> verticeContainer;
	RelationalSchema schema = rbn->getSchema();
	std::vector<std::string> classnames = schema.getClassNames();
	std::vector<std::string> attributnames;
	std::string verticeName, secondVertice;
	
	for(std::vector<std::string>::iterator classsNameIterator = classnames.begin(); classsNameIterator != classnames.end(); ++classsNameIterator){
		attributnames = schema.getClass(*classsNameIterator).getAttributeNames();

		for(std::vector<std::string>::iterator attributNameIterator = attributnames.begin(); attributNameIterator != attributnames.end(); ++attributNameIterator ){
			verticeName = *classsNameIterator;
			verticeName.append(".");
			verticeName.append(*attributNameIterator);
			addVertex(verticeName, verticeContainer);
			
			if(attributNameIterator != attributnames.begin()){
				for(std::vector<std::string>::iterator it = attributnames.begin(); it != attributNameIterator; ++it){
					secondVertice = *classsNameIterator;
					secondVertice.append(".");
					secondVertice.append(*it);
					boost::add_edge(verticeContainer[verticeName], verticeContainer[secondVertice], EdgeProperty(attributeWeight), graph);
				}
			}
		}
	}
	addForeignKeyEdges_multiConnectedAttributs(verticeContainer, FKWeight);
	addProbabilistLink(verticeContainer, probWeight);
}

void PRMDisplay::addVertex(const std::string& verticeName, std::map<std::string, VertexDescriptor>& container){
	VertexDescriptor vd;
	vertexIdPropertyMap = boost::get(&VertexProperties::index, graph);
	positionMap = boost::get(&VertexProperties::point, graph);
	lengthmap = boost::get(&VertexProperties::length, graph);
	vd = boost::add_vertex(graph);
	vertexIdPropertyMap[vd] = verticeName;
	positionMap[vd][0]=1;
	positionMap[vd][1]=1;
	container[verticeName] = vd;
}

void PRMDisplay::addForeignKeyEdges_multiConnectedAttributs(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWweight){
	/***** ajout d'arête pour chaque clé étrangère ****/
	RefSlotsMultimap foreignKeys = rbn->getSchema().getRefSlots();
	std::string classPK, classFK;
	std::vector<std::string> classPKAttributs, classFKAttributes;

	for(RefSlotsMultimap::iterator refSlotIterator = foreignKeys.begin(); refSlotIterator != foreignKeys.end(); ++refSlotIterator){
		classFK = refSlotIterator->first;
		classPK = refSlotIterator->second.second->getName();
		std::vector<std::string> PKClassAttributs = rbn->getSchema().getClass(classPK).getAttributeNames();
		std::vector<std::string> FKClassAttributs = rbn->getSchema().getClass(classFK).getAttributeNames();

		for(std::vector<std::string>::iterator PKAttributsIterator = PKClassAttributs.begin();
		  PKAttributsIterator != PKClassAttributs.end();
		  ++PKAttributsIterator){
			for(std::vector<std::string>::iterator FKAttributsIterator = FKClassAttributs.begin();
			  FKAttributsIterator != FKClassAttributs.end();
			  ++FKAttributsIterator){
				  std::stringstream vertice1, vertice2;
				  vertice1 << classPK << "." << *PKAttributsIterator;
				  vertice2 << classFK << "." << *FKAttributsIterator;
				  boost::add_edge(verticeContainer[vertice1.str()], verticeContainer[vertice2.str()], EdgeProperty(edgeWweight), graph);
			}
		}
	}
}

void PRMDisplay::addProbabilistLink(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWweight){
	std::map<std::string, VertexDescriptor>::iterator verticeIterator;
	std::string vertexName, parentName;
	prm::RBNVariablesSequence variablesSequence;


	for(verticeIterator = verticeContainer.begin(); verticeIterator != verticeContainer.end(); verticeIterator++){
		vertexName = verticeIterator->first;
		
		if(rbn->existsNode(vertexName)){
			variablesSequence = rbn->getParents(vertexName);
			for(unsigned int i = 0; i < variablesSequence.dim(); i++){
				parentName = variablesSequence[i]->toString();
				boost::add_edge(verticeContainer[vertexName], verticeContainer[parentName], EdgeProperty(edgeWweight), graph);
			}
		}
	}
}

void PRMDisplay::RBNToGraph_ArtificialClassVertex(const double attributeWeight, const double FKWeight, const double probWeight){
	/*rechercher toutes classes et tous les attributs*/
	std::map<std::string, VertexDescriptor> verticeContainer;
	RelationalSchema schema = rbn->getSchema();
	std::vector<std::string> classnames = schema.getClassNames();
	std::vector<std::string> attributnames;
	std::string verticeName;
	
	for(std::vector<std::string>::iterator classsNameIterator = classnames.begin(); classsNameIterator != classnames.end(); ++classsNameIterator){
		attributnames = schema.getClass(*classsNameIterator).getAttributeNames();
		addVertex(*classsNameIterator, verticeContainer);

		for(std::vector<std::string>::iterator attributNameIterator = attributnames.begin(); attributNameIterator != attributnames.end(); ++attributNameIterator ){
			verticeName = *classsNameIterator;
			verticeName.append(".");
			verticeName.append(*attributNameIterator);
			
			if (checkFkPkAttribute(*classsNameIterator, *attributNameIterator)){
				addVertex(verticeName, verticeContainer);
				boost::add_edge(verticeContainer[*classsNameIterator], verticeContainer[verticeName], EdgeProperty(attributeWeight), graph);
			}
		}
	}
	addForeignKeyEdges_artificialClassVertex(verticeContainer, FKWeight);
	addProbabilistLink(verticeContainer, probWeight);
}

void PRMDisplay::addForeignKeyEdges_artificialClassVertex(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWweight){
	RefSlotsMultimap foreignKeys = rbn->getSchema().getRefSlots();
	std::string classFrom, classTo, attrFrom;

	for(RefSlotsMultimap::iterator refSlotIterator = foreignKeys.begin(); refSlotIterator != foreignKeys.end(); ++refSlotIterator){
		classFrom = refSlotIterator->first;
		classTo = refSlotIterator->second.second->getName();
		attrFrom = classFrom;
		attrFrom.append(".");
		attrFrom.append(refSlotIterator->second.first.getName());
		
		boost::add_edge(verticeContainer[classFrom], verticeContainer[classTo], EdgeProperty(edgeWweight), graph);
		//boost::add_edge(verticeContainer[attrFrom], verticeContainer[classTo], EdgeProperty(edgeWweight), graph);
	}
}

std::pair<Points, Points> PRMDisplay::getEdgeCoordinates(boost::graph_traits<Graph>::edge_iterator edgeIterator){
	Points beginPoint, endPoint;
	
	beginPoint = positionMap[boost::source(*edgeIterator, graph)];
	endPoint  = positionMap[boost::target(*edgeIterator, graph)];

	return std::pair<Points, Points>(beginPoint, endPoint);
}

std::pair<double, double> PRMDisplay::get2DLinearEquation(Points point1, Points point2){
	double a, b = 0 ;
	try {
		a = (point2[1] - point1[1]) / (point2[0] - point1[0]);
		b = point1[1] - (a * point1[0]);
	} catch (std::exception& e) {
		std::cerr << e.what() << " in get2DLinearEquation function" << std::endl;
	}

	return std::pair<double, double>(a,b);
}

bool PRMDisplay::isBetween(double xcoordinate, double x1, double x2){
	bool result = false;

	if( x1 < x2 ){
		if( (xcoordinate >= x1) && (xcoordinate <= x2) ){
			result = true;
		}
	} else {
		if( (xcoordinate <= x1) && (xcoordinate >= x2) ){
			result = true;
		}
	}
	return result;
}

Points& PRMDisplay::reversePointCoordinate(Points& p){
	double temp = p[1];
	p[1] = p[0];
	p[0] = temp;
	return p;
}

int PRMDisplay::getNbCrossing(){
	int result = 0;
	boost::graph_traits<Graph>::edge_iterator edgeIterator1, end, edgeIterator2;
	Points beginPoint_edge1, endPoint_edge1, beginPoint_edge2, endPoint_edge2, tmp1, tmp2;
	double a1, a2, b1, b2, xcommun;
	VertexDescriptor v1,v2,v3,v4;

	for(boost::tie(edgeIterator1, end) = boost::edges(graph); edgeIterator1 != boost::prior(end); edgeIterator1++){
		boost::tie(beginPoint_edge1, endPoint_edge1) = getEdgeCoordinates(edgeIterator1); // arête à comparer avec le reste de la liste
		
		v1 = boost::source(*edgeIterator1, graph);
		v2 = boost::target(*edgeIterator1, graph);

		for (edgeIterator2 = boost::next(edgeIterator1) ; edgeIterator2 != end; edgeIterator2++) {
			boost::tie(beginPoint_edge2, endPoint_edge2) = getEdgeCoordinates(edgeIterator2);
			
			v3 = boost::source(*edgeIterator2, graph);
			v4 = boost::target(*edgeIterator2, graph);

			if((v1 != v3) && (v1 != v4) && (v2 != v3) && (v2 != v4)){
				//On veut connaitre les équations des 2 droites
				//rechercher les coefficients directeur a1 et a2
				if((beginPoint_edge1[0] != endPoint_edge1[0]) && (beginPoint_edge2[0] != endPoint_edge2[0])){ 
					//std::cout << "toto1 " <<beginPoint_edge1[0]<<"_" << endPoint_edge1[0]<<"_"<< beginPoint_edge2[0]<<"_"<<endPoint_edge2[0] << std::endl;
					boost::tie(a1, b1) = get2DLinearEquation(beginPoint_edge1, endPoint_edge1);
					boost::tie(a2, b2) = get2DLinearEquation(beginPoint_edge2, endPoint_edge2);
				} else { // droite verticale x1 = x2 il faut changer le repère
					if((beginPoint_edge1[1] != endPoint_edge1[1]) && (beginPoint_edge2[1] != endPoint_edge2[1])){
						boost::tie(a1, b1) = get2DLinearEquation(reversePointCoordinate(beginPoint_edge1), reversePointCoordinate(endPoint_edge1));
						boost::tie(a2, b2) = get2DLinearEquation(reversePointCoordinate(beginPoint_edge2), reversePointCoordinate(endPoint_edge2));
					} else { //perpendicularité horizontal/vertical,
						a1 = a2 = 0.0;
						if(beginPoint_edge1[0] == endPoint_edge1[0]) { //arête1 verticale
							if(isBetween(beginPoint_edge1[0], beginPoint_edge2[0], endPoint_edge2[0]) &&
								isBetween(beginPoint_edge2[1], beginPoint_edge1[1], endPoint_edge1[1])) {
									result++;	
							}
						} else { //arête 2 verticale
							if(isBetween(beginPoint_edge2[0], beginPoint_edge1[0], endPoint_edge1[0]) &&
								isBetween(beginPoint_edge1[1], beginPoint_edge2[1], endPoint_edge2[1])) {
									result++;

							}
						}
					}
				}
			
				//si a1 = a2 alors les segments ne se croisent pas.
				if(a1 != a2) {
					xcommun = (b2-b1)/(a1-a2);
					if( isBetween(xcommun, beginPoint_edge1[0], endPoint_edge1[0]) && isBetween(xcommun, beginPoint_edge2[0], endPoint_edge2[0]) ){
						result++;
					}
				}
			}
		}
		
	}

	return result;
}

int PRMDisplay::getMaxCrossing() {
	int call, cimpossible, m, n, degree;
	n = boost::num_vertices(graph);
	m = boost::num_edges(graph);
	boost::graph_traits<Graph>::vertex_iterator iterator, end;
	
	call = m*(m-1)/2;
	cimpossible = 0;
	for(boost::tie(iterator, end) = vertices(graph) ; iterator != end ; iterator++ ){
		degree = boost::in_degree(*iterator, graph);
		cimpossible +=  (degree * (degree-1));
	}
	cimpossible = cimpossible / 2;
	return call - cimpossible;

}

double PRMDisplay::getCrossingScore(){
	int cmax = getMaxCrossing();
	if(cmax > 0){
		return 1. - (static_cast<double>(getNbCrossing()) / static_cast<double>(cmax));
	} else {
		return 1.;
	}
}

void PRMDisplay::initGraph(){
	
	VertexDescriptor vd1, vd2, vd3, vd4, vd5, vd6, vd7, vd8;

	vertexIdPropertyMap = boost::get(&VertexProperties::index, graph);
	positionMap = boost::get(&VertexProperties::point, graph);
	
	vd1 = boost::add_vertex(graph);
	vertexIdPropertyMap[vd1] = "0_0";
	
	positionMap[vd1][0]=0;
	positionMap[vd1][1]=0;
	

	vd2 = boost::add_vertex(graph);
	vertexIdPropertyMap[vd2] = "10_10";
	positionMap[vd2][0]=10;
	positionMap[vd2][1]=10;

	vd3 = boost::add_vertex(graph);
	vertexIdPropertyMap[vd3] = "0_10";
	positionMap[vd3][0]=0;
	positionMap[vd3][1]=10;

	vd4 = boost::add_vertex(graph);
	vertexIdPropertyMap[vd4] = "10_0";
	positionMap[vd4][0]=10;
	positionMap[vd4][1]=0;

	vd5 = boost::add_vertex(graph);
	vertexIdPropertyMap[vd5] = "1_1";
	positionMap[vd5][0]=1;
	positionMap[vd5][1]=1;

	vd6 = boost::add_vertex(graph);
	vertexIdPropertyMap[vd6] = "1_9";
	positionMap[vd6][0]=1;
	positionMap[vd6][1]=9;

	vd7 = boost::add_vertex(graph);
	vertexIdPropertyMap[vd7] = "2_8";
	positionMap[vd7][0]=2;
	positionMap[vd7][1]=8;

	vd8 = boost::add_vertex(graph);
	vertexIdPropertyMap[vd8] = "9_8";
	positionMap[vd8][0]=9;
	positionMap[vd8][1]=8;
	

	boost::add_edge(vd1, vd2, EdgeProperty(1), graph);
	boost::add_edge(vd3, vd4, EdgeProperty(1), graph);
	boost::add_edge(vd7, vd8, EdgeProperty(1), graph);
	boost::add_edge(vd5, vd6, EdgeProperty(1), graph);
}