
#include "PRMDisplay.h"
#include "RBN.h"
#include <Shapes.h>
#include <sstream>


using namespace LibBoard;
using namespace prm;

PRMDisplay::PRMDisplay(const boost::shared_ptr<prm::RBN> rbn, const Graph& graph):rbn(rbn), graph(graph){
	board.clear( Color(255,255,255) );
	board.setUnit(8, Board::UPoint);
	board.setFontSize(14);
	board.setPenColorRGBi( 255, 0, 0);
	board.setLineWidth(0.5);

	delta=1.0;
	
}

std::vector<double> PRMDisplay::displaySize(){
	std::vector<double> result;
	double minX=0, maxX=0,minY=0, maxY=0;
	minX=listRect[0].x; 
	minY=listRect[0].y; 
	for (std::vector<RectClass>::iterator it = listRect.begin();it != listRect.end(); ++it){
		minY=min(it->y, minY);
		maxY=max(it->y+it->hei, maxY);
		minX=min(it->x, minX);
		maxX=max(it->x+it->len, maxX);
	}
	result.push_back(maxX-minX + 20);
	result.push_back(maxY-minY + 20);
	return result;
}

void PRMDisplay::placeVertex(int red, int green, int blue){
	std::map<std::string, Color> colorclasse; 
	int nbclasse=5;
	srand( static_cast<unsigned int>( time(0) ) );
	boost::graph_traits<Graph>::vertex_iterator i, end;
    for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
        std::cout << "ID: (" << vertexIdPropertyMap[*i] << ") x: " << positionMap[*i][0] << " y: " << positionMap[*i][1] << "\n";
		std::string classename=vertexIdPropertyMap[*i].substr(0,vertexIdPropertyMap[*i].find("."));
		std::string attribute=vertexIdPropertyMap[*i].substr(vertexIdPropertyMap[*i].find(".")+1, vertexIdPropertyMap[*i].length());
		// couleur
		//makeVertexColor(classename);
		if (attribute.length()<=7){
			lengthmap[*i]=attribute.length()/1.35f;
		}else{
			lengthmap[*i]=attribute.length()/1.65f;
		}
		if (vertexIdPropertyMap[*i].find(".")!=std::string::npos) {
			//if (checkFkPkAttribute(classename, attribute)){
					//board.setPenColorRGBi( 255, 255, 255);
					//board.fillEllipse(positionMap[*i][0]+lengthmap[*i]/2, positionMap[*i][1]+0.75, lengthmap[*i]/2	, 1, 3 );
					board.setPenColorRGBi(red, green, blue);
					board.drawText(positionMap[*i][0]+1,positionMap[*i][1]+.5, vertexIdPropertyMap[*i].substr(vertexIdPropertyMap[*i].find(".")+1, vertexIdPropertyMap[*i].length()), 15000 );
					lengthmap[*i]+=1;
					board.drawEllipse(positionMap[*i][0]+lengthmap[*i]/2, positionMap[*i][1]+0.75, lengthmap[*i]/2	, 1 );
					

					//board.drawRectangle(positionMap[*i][0],positionMap[*i][1],lengthmap[*i],1.5 );
			//}
		}
	}
	//placeClasse();

}

bool PRMDisplay::checkFkPkAttribute(const std::string& classename,const std::string& attribute){
	if (!rbn->getSchema().isFKAttribute(classename, attribute)){
				if (!rbn->getSchema().getClass(classename).isPK(attribute)) {
					return true; 
				}
	} 
	return false;
}

/*void PRMDisplay::makeVertexColor(const std::string& classename){
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
*/
void PRMDisplay::placeClasse(int red, int green, int blue){
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
						if ((it->y<=positionMap[*i][1]) && ((it->y)+(it->hei)>=(positionMap[*i][1]+1.5f))) {

						}else if (it->y>positionMap[*i][1]){
							it->hei+=std::abs(it->y-positionMap[*i][1]);
						}else if ((it->y)+(it->hei)<(positionMap[*i][1]+1.5f)){
							it->hei+=std::abs(positionMap[*i][1]+1.5f-(it->y+it->hei));
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
	drawClass(red, green, blue);
}

void PRMDisplay::drawClass(int red, int green, int blue){
	// je dessine les rectangles de classe avec la bonne couleur de pinceau
	board.setPenColorRGBi( red, green, blue);
	for (std::vector<RectClass>::iterator it = listRect.begin();it != listRect.end(); ++it){
		it->y-=delta; it->hei+=delta+delta+1.5f;it->x-=delta;it->len+=delta+delta;
		board.drawRectangle(it->x,it->y+it->hei,it->len,it->hei);	
		/*board.drawLine(it->x,it->y,it->x+it->len,it->y );
		board.drawLine(it->x,it->y,it->x,it->hei+it->y );
		board.drawLine(it->x,it->hei+it->y,it->x+it->len,it->hei+it->y );
		board.drawLine(it->x+it->len,it->y,it->x+it->len,it->hei+it->y );*/
		board.drawText(it->x+.1,it->y+it->hei-1, it->classeName,10 );
		
	}
}

void PRMDisplay::placeRelationnalLink(int red, int green, int blue){
	board.setPenColorRGBi( red, green, blue);
	std::map<std::string, std::string> result; // model (classe.FK, classe1.PK)
	RefSlotsMultimap refSlots = rbn->getSchema().getRefSlots();
	// Fisrt: get all the link between PK and FK	
	for(RefSlotsMultimap::iterator rb = refSlots.begin(); rb != refSlots.end(); ++rb){
		std::string classPK=rb->second.second->getName().c_str();
		std::string classFK=rb->first.c_str();
		double x1,x2,y1,y2;
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
		/*s'il ya chevauchement des 2 extremités alors je met 2 coudes du centre du rectangle 1 au centre du rectangle 2
		  s'il ya chevauchement d'une extremité alors si il y a plus de la moitié du rectangle 1 en dehors du rectangle 2 
																alors je fait un seul coude
													sinon je met 2 coudes du centre du rectangle 1 au centre du rectangle 2
		   si il n'ya pas de chevauchement alors je met un coude*/
							
		if (x1<x2 && x1+PK.len>x2+FK.len){// chevauchement en X des 2 cotés
			x1+=PK.len/2;x2+=FK.len/2;
			if (y1<y2){ //le rectangle 1 est en dessous du rectangle 2
				x1+=PK.len/2; y1+=PK.hei; x2+=FK.len/2;
				drawRelationnalLink(3,x1,y1,x2,y2 );
			}else if (y1>y2+FK.hei){ //le rectangle 1 est en dessus du rectangle 2
				x1+=PK.len/2; y2+=FK.hei; x2+=FK.len/2;
				drawRelationnalLink(3,x1,y1,x2,y2 );
			}
		}else if ((x2<= x1  && x1< x2+FK.len) || (x2< x1+PK.len && x1+PK.len <= x2+FK.len)){ // chevauchement en X d'un coté ou de l'autre
			if (y1<y2){ //le rectangle 1 est en dessous du rectangle 2
				if (x1<x2 && x1+PK.len/2<=x2 ){
					x1+=PK.len/3;y1+=PK.hei; y2+=FK.hei/3;
					drawRelationnalLink(4,x1,y1,x2,y2 );
				}else if(x2+FK.len<=x1+PK.len/2) {
					x1+=2*PK.len/3;y1+=PK.hei; y2+=FK.hei/3; x2+=FK.len;
					drawRelationnalLink(4,x1,y1,x2,y2 );
				}else{
					x1+=PK.len/2; y1+=PK.hei; x2+=FK.len/2;
					drawRelationnalLink(3,x1,y1,x2,y2 );
				}
			}else if (y1>y2+FK.hei){ //le rectangle 1 est en dessus du rectangle 2
				if (x1<x2 && x1+PK.len/2<=x2 ){
					x1+=PK.len/3; y2+=2*FK.hei/3;
					drawRelationnalLink(4,x1,y1,x2,y2 );
				}else if(x2+FK.len<=x1+PK.len/2) {
					x1+=2*PK.len/3;y2+=2*FK.hei/3; x2+=FK.len;
					drawRelationnalLink(4,x1,y1,x2,y2 );
				}else{
					x1+=PK.len/2; y2+=FK.hei; x2+=FK.len/2;
					drawRelationnalLink(3,x1,y1,x2,y2 );
				}
			}

		}else if (y1<y2 && y1+PK.hei>y2+FK.len){// chevauchement en Y des 2 cotés
			if (x1<x2){ //le rectangle 1 est a gauche du rectangle 2
				x1+=PK.len;y1+=PK.hei/2;y2+=FK.len/2;
				drawRelationnalLink(2,x1,y1,x2,y2 );
			}else if (x1>x2){ //le rectangle 1 est en dessus du rectangle 2
				y1+=PK.hei/2;y2+=FK.len/2;x2+=FK.len;
				drawRelationnalLink(2,x1,y1,x2,y2 );
			}
		}else if ((y2<= y1  && y1< y2+FK.hei) || (y2< y1+PK.hei && y1+PK.hei <= y2+FK.hei)){ // chevauchement en Y
			if (x1<x2){ //le rectangle 1 est en gauche du rectangle 2
				if (y1<y2 && y1+PK.hei/2<y2 ){
					x1+=PK.len;y1+=PK.hei/3;x2+=FK.len/3;
					drawRelationnalLink(1,x1,y1,x2,y2 );
				}else if(y2+FK.hei<y1+PK.hei/2) {
					x1+=PK.len;y1+=2*PK.hei/3;x2+=FK.len/3;y2+=FK.hei;
					drawRelationnalLink(1,x1,y1,x2,y2 );
				}else{
					x1+=PK.len;y1+=PK.hei/2;y2+=FK.len/2;
					drawRelationnalLink(2,x1,y1,x2,y2 );
				}
			}else if (x1>x2){ //le rectangle 1 est en droite du rectangle 2
				if (y1<y2 && y1+PK.hei/2<y2 ){
					y1+=PK.hei/3;x2+=2*FK.len/3;
					drawRelationnalLink(1,x1,y1,x2,y2 );
				}else if(y2+FK.hei<y1+PK.hei/2) {
					y1+=2*PK.hei/3;x2+=2*FK.len/3;y2+=FK.hei;
					drawRelationnalLink(1,x1,y1,x2,y2 );
				}else{
					y1+=PK.hei/2;y2+=FK.len/2;x2+=FK.len;
					drawRelationnalLink(2,x1,y1,x2,y2 );
				}
			}
		}else{ // pas de chevauchement
			if (x1+PK.hei<x2){//1 gauche de 2
				if (y1+PK.hei<=y2){ //1 dessous 2
					x1+=2*PK.len/3; y1+=PK.hei; y2+=FK.hei/3;
					drawRelationnalLink(4,x1,y1,x2,y2 );
				}else{// if (y1>=y2+FK.hei){//1 dessus 2
					x1+=PK.len; y1+=PK.hei/3; y2+=FK.hei;x2+=FK.len/3;
					drawRelationnalLink(1,x1,y1,x2,y2 );
				}
			}else{//1 droite de 2
				if (y1+PK.hei<=y2){ //1 dessous 2
					x1+=PK.len/3; y1+=PK.hei; x2+=FK.len;y2+=FK.hei/3;
					drawRelationnalLink(4,x1,y1,x2,y2 );
				}else{// if (y1>=y2+FK.hei){//1 dessus 2
					 y1+=PK.hei/3; y2+=FK.hei;x2+=2*FK.len/3;
					 drawRelationnalLink(1,x1,y1,x2,y2 );
				}
			}

		}
	}
}

void PRMDisplay::drawRelationnalLink(int nbCoude, double x1,double y1,double x2,double y2 ){
	double tmpX, tmpY;
	switch (nbCoude){
	case 1: //forme: -|
		board.drawLine(x1,y1,x2,y1);
		board.drawLine(x2,y1,x2,y2);
		break; 
	case 2: //forme -|_
		tmpX=std::abs(x1-x2)/2;
		if (x1<x2){tmpX+=x1;} else {tmpX+=x2;}
		board.drawLine(x1,y1,tmpX,y1);
		board.drawLine(tmpX,y1,tmpX,y2);
		board.drawLine(tmpX,y2,x2,y2);

		break;

	case 3://forme '-,
		tmpY=std::abs(y1-y2)/2;
		if (y1<y2){tmpY+=y1;} else {tmpY+=y2;}
		board.drawLine(x1,y1,x1,tmpY);
		board.drawLine(x2,tmpY,x1,tmpY);
		board.drawLine(x2,tmpY,x2,y2);
		break;

	case 4://forme |_
		board.drawLine(x1,y1,x1,y2);
		board.drawLine(x1,y2,x2,y2);
		break;

	}
}

double PRMDisplay::distanceBetweenDot(double x1,double y1,double x2,double y2 ){
	return std::sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}


double PRMDisplay::minDistance4point(double a,double b,double c,double d ){
	double mini; 
	mini=min(a,b);
	mini=min(mini,c);
	mini=min(mini,d);
	return mini;
}

std::vector<double> PRMDisplay::intersectionEllispeLine(double axe1, double axe2, double x1, double y1, double x2, double y2, int extremite) {
		double X1,Y1,X2,Y2, a, c,d;
		
		//changement de repere
		/*X=x-x1;
		Y=y-y1;*/
		//Coefficient directeur de la droite	
		if(extremite==1){
			a=(y2-y1)/(x2-x1);
		}else{
			a=(y1-y2)/(x1-x2);
		}
		//b=y1-(a*x1);
		//coefficient ellispe
		c=axe1*axe1;
		d=axe2*axe2;
		//calcul de X et de Y
		X1 =axe1*axe2 / std::sqrt( (axe2*axe2) + (axe1*axe1)*(a*a) );
		X2 =-axe1*axe2 / std::sqrt( (axe2*axe2) + (axe1*axe1)*(a*a) );
		Y1 = a*X1;
		Y2 = a*X2;
		std::vector<double> result;

		if (extremite==1){
			result.push_back(X1+x1);
			result.push_back(Y1+y1);
			result.push_back(X2+x1);
			result.push_back(Y2+y1);
			
		}else{
			result.push_back(X1+x2);
			result.push_back(Y1+y2);
			result.push_back(X2+x2);
			result.push_back(Y2+y2);
		}

		//std::vector<double> results (result, result + sizeof(result) / sizeof(double) );
		return result;
}

void PRMDisplay::placeProbabilistLink(int red, int green, int blue){ 
	board.setPenColorRGBi( red, green, blue);
	boost::graph_traits<Graph>::vertex_iterator i, end;
	RBNVariablesSequence seq;
	std::string aggregat;
	 for (boost::tie(i, end) = boost::vertices(graph); i != end; i++) {
		
		if (rbn->existsNode(vertexIdPropertyMap[*i])){
			seq=rbn->getParents(vertexIdPropertyMap[*i]);
			if (seq.dim()>0) {
				for(unsigned int j = 0; j < seq.dim(); j++){
					boost::graph_traits<Graph>::vertex_iterator i1, end1;
					for (boost::tie(i1, end1) = boost::vertices(graph); i1 != end1; ++i1) {
						if (vertexIdPropertyMap[*i1].compare(dynamic_pointer_cast<IRBNSimpleVariable>(seq[j])->getBaseName())==0){
							aggregat=Aggregator::getAggregatorName(seq[j]->getAggregatorType());
							double x1,x2,y1,y2;
							x1=positionMap[*i1][0]+lengthmap[*i1]/2;
							y1=positionMap[*i1][1]+.75;
							x2=positionMap[*i][0]+lengthmap[*i]/2;
							y2=positionMap[*i][1]+.75;
							std::vector<double> intersec2=intersectionEllispeLine(lengthmap[*i]/2, 1, x1, y1,x2,y2,2 ) ;

							std::vector<double> intersec1=intersectionEllispeLine(lengthmap[*i1]/2, 1, x1, y1, x2, y2, 1); 
							
							std::cout<<"1 -> ["<<intersec1[0]<<","<<intersec1[1]<<","<<intersec1[2]<<","<<intersec1[3]<<"]\n  ";
							std::cout<<"2 -> ["<<intersec2[0]<<","<<intersec2[1]<<","<<intersec2[2]<<","<<intersec2[3]<<"]\n  ";
							
							if ((x1>x2)&&(y1<y2)){
								x1=min(intersec1[0],intersec1[2]);
								y1=max(intersec1[1],intersec1[3]);
								x2=max(intersec2[0],intersec2[2]);
								y2=min(intersec2[1],intersec2[3]);
								
							}else if ((x1>x2)&&(y1>y2)){ 
								x1=min(intersec1[0],intersec1[2]);
								y1=min(intersec1[1],intersec1[3]);
								x2=max(intersec2[0],intersec2[2]);
								y2=max(intersec2[1],intersec2[3]);								
								
							}else if ((x1<x2)&&(y1>y2)){
								x1=max(intersec1[0],intersec1[2]);
								y1=min(intersec1[1],intersec1[3]);
								x2=min(intersec2[0],intersec2[2]);
								y2=max(intersec2[1],intersec2[3]);
								
							}else if ((x1<x2)&&(y1<y2)){
								x1=max(intersec1[0],intersec1[2]);
								y1=max(intersec1[1],intersec1[3]);
								x2=min(intersec2[0],intersec2[2]);
								y2=min(intersec2[1],intersec2[3]);
							}
							drawProbabilistLink(aggregat,x1,y1, x2,y2);
							break;
						}
					}
				}
			}
		}
	 }
			
	
}

void PRMDisplay::drawProbabilistLink(const std::string aggregat, double x1,double y1,double x2,double y2 ){
	if(aggregat.compare("NO")==0){
		board.setLineWidth(0.5);
		board.drawArrow(x1,y1, x2,y2);
	}else{
		double centreX, centreY,tmpX1, tmpY1, a, b, tmpX2, tmpY2;
		
		board.setLineWidth(1);
		centreX=(x1+x2)/2; 
		centreY=(y1+y2)/2; 
		board.setFontSize(12);

		if (x1==x2){
			tmpX1=centreX; tmpX2=centreX;
			if (y1<y2){ tmpY1=centreY-.75;  tmpY2=centreY+.75; }
			else{tmpY2=centreY-.75;  tmpY1=centreY+.75;}
		}else {
			a=(y2-y1)/(x2-x1);
			b=y1-(a*x1);
			if (std::abs(a)<.35 && std::abs(a)>0) {
				if (x1>x2) { 
					tmpX1=centreX+(aggregat.length()/2);  
					tmpY1=a*tmpX1+b;
					tmpX2=centreX-(aggregat.length()/2);  
					tmpY2=a*tmpX2+b;
				}else {
					tmpX1=centreX-(aggregat.length()/2);  
					tmpY1=a*tmpX1+b;
					tmpX2=centreX+(aggregat.length()/2);  
					tmpY2=a*tmpX2+b;
				}
			}else{
				if (y1==y2){ tmpX1=centreX-(aggregat.length()/2);  tmpX2=centreX+(aggregat.length()/2);tmpY1=centreY; tmpY2=centreY;}
				else if (y1>y2) { 
					tmpY1=centreY+.75;  
					tmpX1=(tmpY1-b)/a;
					tmpY2=centreY-.75;  
					tmpX2=(tmpY2-b)/a;
				}else {
					tmpY1=centreY-.75;  
					tmpX1=(tmpY1-b)/a;
					tmpY2=centreY+.75;  
					tmpX2=(tmpY2-b)/a;
				}
			}
		}
			
		board.drawRectangle(centreX-(aggregat.length()/2), centreY+0.75, aggregat.length(), 1.5);
		board.drawText(centreX-(aggregat.length()/2)+.2, centreY-0.4, aggregat);
		board.drawLine(x1,y1,tmpX1, tmpY1);
		board.drawArrow(tmpX2, tmpY2, x2,y2);
	}
	board.setFontSize(14);

	
}

void PRMDisplay::display(const std::string& path, const std::string& name){
	std::string completepath;
	completepath.append(path);
	completepath.append("\\");
	completepath.append(name);
	//completepath.append(".svg");
	//board.translate(5,5);
	std::vector<double> sizeXY;
	sizeXY=displaySize();
	std::cout <<sizeXY[0]<<","<<sizeXY[1]<<std::endl;
	board.saveSVG(completepath.append(".svg").c_str(),200,200);
	//board.saveFIG(completepath.append(".fig").c_str(),200,200, 10.0);
	//board.saveFIG(completepath.append(".eps").c_str(),200,200, 10.0);
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

void PRMDisplay::prevKamada(const double edgelength, Graph& g, PositionMap& position){
	boost::minstd_rand gen;
	Topology topology(gen, edgelength*5);

	WeightPropertyMap weightPropertyMap = boost::get(&EdgeProperty::weight, g);
	boost::circle_graph_layout(g, position, edgelength*5);
	
	bool result = boost::kamada_kawai_spring_layout(g, 
                                       position,
                                       weightPropertyMap,
									   topology,
                                       edge_length(edgelength),
									   boost::layout_tolerance<>());
	
	displayKamadaCheck(result);
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

std::pair<Points, Points> PRMDisplay::getExtremGraphPoint(Graph& g){
	boost::graph_traits<Graph>::vertex_iterator i, end;
	Topology topology;
	PositionMap position = boost::get(&VertexProperties::point, g);

	Points max_point, min_point = position[*vertices(g).first];

	for (boost::tie(i, end) = boost::vertices(g); i != end; ++i) {
		min_point = topology.pointwise_min(min_point, position[*i]);
		max_point = topology.pointwise_max(max_point, position[*i]);
		std::cout << position[*i][0] << " et "<< position[*i][1]<< std::endl;
	}
	
	return std::pair<Points, Points>(min_point, max_point);
}

void PRMDisplay::adjustDisplayAfterKamada(const double length){
	boost::graph_traits<Graph>::vertex_iterator i, end;
	Points min_point;
	const int margin = 5;

	min_point = getExtremGraphPoint(this->graph).first;

	for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
		positionMap[*i][0] += margin - min_point[0];
		positionMap[*i][1] += margin - min_point[1];
	} 
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
	positionMap = boost::get(&VertexProperties::point, graph);
	addVertex(verticeName, container, graph, positionMap);
}

void PRMDisplay::addVertex(const std::string& verticeName, std::map<std::string, VertexDescriptor>& container, Graph& g, PositionMap& position){
	VertexDescriptor vd;
	vertexIdPropertyMap = boost::get(&VertexProperties::index, g);
	lengthmap = boost::get(&VertexProperties::length, g);
	vd = boost::add_vertex(g);
	vertexIdPropertyMap[vd] = verticeName;
	position[vd][0]=1;
	position[vd][1]=1;
	container[verticeName] = vd;
}

void PRMDisplay::addForeignKeyEdges_multiConnectedAttributs(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWeight){
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
				  boost::add_edge(verticeContainer[vertice1.str()], verticeContainer[vertice2.str()], EdgeProperty(edgeWeight), graph);
			}
		}
	}
}

void PRMDisplay::addProbabilistLink(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWeight){
	std::map<std::string, VertexDescriptor>::iterator verticeIterator;
	std::string vertexName, parentName, key;
	prm::RBNVariablesSequence variablesSequence;
	std::map<std::string, unsigned int> probabilisticConnection;
	double weight, weighting = 0. ; 

	//probabilisticConnection = computeProbabilisticConnection();
	
	for(verticeIterator = verticeContainer.begin(); verticeIterator != verticeContainer.end(); verticeIterator++){
		vertexName = verticeIterator->first;
		std::cout << vertexName << std::endl;
		if(rbn->existsNode(vertexName)){
			variablesSequence = rbn->getParents(vertexName);
			for(unsigned int i = 0; i < variablesSequence.dim(); i++){
				parentName = variablesSequence[i]->toString();
				key = concat2SortedClass(vertexName, parentName);
				/*Déterminer une fonction inverse pour pondérer le poids de l'arête*/
				//weighting = 1.0 / static_cast<double>(probabilisticConnection.at(key));
				weight = edgeWeight * (1 + weighting);
				boost::add_edge(verticeContainer[vertexName], verticeContainer[parentName], EdgeProperty(weight), graph);
			}
		}
	}
}

std::string PRMDisplay::concat2SortedClass(std::string str1, std::string str2){
	std::string result;

	str1 = str1.substr(0, str1.find("."));
	str2 = str2.substr(0, str2.find("."));

	if(str1.compare(str2) < 0 ){
		result = str1;
		result.append(".");
		result.append(str2);
	} else {
		result = str2;
		result.append(".");
		result.append(str1);
	}
	return result;
}

std::map<std::string, unsigned int> PRMDisplay::computeProbabilisticConnection(){
	boost::graph_traits<Graph>::vertex_iterator verticeIterator, end;
	std::map<std::string, unsigned int> result;
	std::string vertexName, parentName, key;
	prm::RBNVariablesSequence variablesSequence;
	vertexIdPropertyMap = boost::get(&VertexProperties::index, graph);

	for( boost::tie(verticeIterator, end) = boost::vertices(graph); verticeIterator != end; verticeIterator++){
		vertexName = vertexIdPropertyMap[*verticeIterator];
		if(rbn->existsNode(vertexName)){
			variablesSequence = rbn->getParents(vertexName);
			for(unsigned int i = 0; i < variablesSequence.dim(); i++){
				parentName = variablesSequence[i]->toString();
				key = concat2SortedClass(vertexName,parentName);
				if(result.count(key) == 1){
					result[key] += 1;
				} else {
					result[key] = 1;
				}
			}
		}
	}
	return result;
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

void PRMDisplay::addForeignKeyEdges_artificialClassVertex(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWeight){
	RefSlotsMultimap foreignKeys = rbn->getSchema().getRefSlots();
	std::string classFrom, classTo, attrFrom;

	for(RefSlotsMultimap::iterator refSlotIterator = foreignKeys.begin(); refSlotIterator != foreignKeys.end(); ++refSlotIterator){
		classFrom = refSlotIterator->first;
		classTo = refSlotIterator->second.second->getName();
		attrFrom = classFrom;
		attrFrom.append(".");
		attrFrom.append(refSlotIterator->second.first.getName());
		
		boost::add_edge(verticeContainer[classFrom], verticeContainer[classTo], EdgeProperty(edgeWeight), graph);
		//boost::add_edge(verticeContainer[attrFrom], verticeContainer[classTo], EdgeProperty(edgeWweight), graph);
	}
}

void PRMDisplay::RBNToGraph_preComputedClassVertex(const double attributeWeight, const double probWeight){
	/*rechercher toutes classes et tous les attributs*/
	std::map<std::string, VertexDescriptor> verticeContainer, facticeContainer;
	RelationalSchema schema = rbn->getSchema();
	std::vector<std::string> classnames = schema.getClassNames();
	std::vector<std::string> attributnames;
	std::string verticeName;
	std::map<std::string, std::pair<double, double>> boundingLengths;
	Graph* aClass;
	PositionMap position;

	for(std::vector<std::string>::iterator classsNameIterator = classnames.begin(); classsNameIterator != classnames.end(); ++classsNameIterator){
		aClass = new Graph();
		position = boost::get(&VertexProperties::point, *aClass);

		attributnames = schema.getClass(*classsNameIterator).getAttributeNames();
		addVertex(*classsNameIterator, facticeContainer, *aClass, position);
		addVertex(*classsNameIterator, verticeContainer);
		
		for(std::vector<std::string>::iterator attributNameIterator = attributnames.begin(); attributNameIterator != attributnames.end(); ++attributNameIterator ){
			verticeName = *classsNameIterator;
			verticeName.append(".");
			verticeName.append(*attributNameIterator);
			
			if (checkFkPkAttribute(*classsNameIterator, *attributNameIterator)){
				addVertex(verticeName, facticeContainer, *aClass, position);
				addVertex(verticeName, verticeContainer);
				boost::add_edge(facticeContainer[*classsNameIterator], facticeContainer[verticeName], EdgeProperty(attributeWeight), *aClass);
				boost::add_edge(verticeContainer[*classsNameIterator], verticeContainer[verticeName], EdgeProperty(attributeWeight), graph);
			}
		}
		prevKamada(1., *aClass, position);
		boundingLengths[*classsNameIterator] = computeBoundingLength(*aClass);
		
		delete aClass;
	}

	addForeignKeyEdges_artificialClassVertex(verticeContainer, boundingLengths);
	addProbabilistLink(verticeContainer, probWeight);
}

void PRMDisplay::addForeignKeyEdges_artificialClassVertex(std::map<std::string, VertexDescriptor>& verticeContainer, const std::map<std::string, std::pair<double, double>>& boundingLength){
	RefSlotsMultimap foreignKeys = rbn->getSchema().getRefSlots();
	std::string classFrom, classTo, attrFrom;
	double edgeWeight = 0;

	for(RefSlotsMultimap::iterator refSlotIterator = foreignKeys.begin(); refSlotIterator != foreignKeys.end(); ++refSlotIterator){
		classFrom = refSlotIterator->first;
		classTo = refSlotIterator->second.second->getName();

		edgeWeight = computeEdgeWeight(classFrom, classTo, boundingLength);
		
		boost::add_edge(verticeContainer[classFrom], verticeContainer[classTo], EdgeProperty(edgeWeight), graph);
	}
}

double PRMDisplay::computeEdgeWeight(const std::string& classFrom, const std::string& classTo, const std::map<std::string, std::pair<double, double>>& boundingLength){
	double classFromWidth, classFromHeight, classToWidth, classToHeight, ab, ac = 0.0 ;
	std::pair<double, double> classFromLengths, classToLengths;
	const double margin = delta;

	classFromWidth = boundingLength.at(classFrom).first;
	classFromHeight = boundingLength.at(classFrom).second;
	classToWidth = boundingLength.at(classTo).first;
	classToHeight = boundingLength.at(classTo).second;

	ab = ( classFromHeight + classToHeight + margin ) / 2.;
	ac = ( classFromHeight + classToHeight + margin ) / 2.;

	return sqrt( ab * ab + ac * ac );
	
}

std::pair<double, double> PRMDisplay::computeBoundingLength(Graph& g){
	Points minPoint, maxPoint;
	double width, height;
	boost::tie(minPoint, maxPoint) = getExtremGraphPoint(g);
	std::cout << minPoint[0] << " " << minPoint[1] << " " << maxPoint[0] << " " << maxPoint[1] << std::endl;
	width = maxPoint[0] - minPoint[0] + delta;
	height = maxPoint[1] - minPoint[1] + delta;

	return std::pair<double, double>(width, height);
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