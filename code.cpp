#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <time.h>

class place{
private:
	char printed,value;
public:
	place(): value(0),printed(0){};
	char p(){return printed;}
	char v(){return value;}
	void set(char c){
		value=c;
		printed='.';
	}
	bool flag(){
		printed='f';
		return value == '*';
	}
	bool unflag(){
		printed='.';
		return value=='*';
	}
	bool discover(){
		if(value=='*') return true;
		printed=value;
		return false;
	}
	void print() {printf("%c\t%c\n",printed,value);}
};

int count(place** p,int x,int y,int s){
	int ret=0;
	if(x>0){
		if(y>0) ret+=p[x-1][y-1].v()=='*';
		if(y<s-1) ret+=p[x-1][y+1].v()=='*';
		ret+=p[x-1][y].v()=='*';
	}
	if(x<s-1){
		if(y>0) ret+=p[x+1][y-1].v()=='*';
		if(y<s-1) ret+=p[x+1][y+1].v()=='*';
		ret+=p[x+1][y].v()=='*';
	}
	if(y>0) ret+=p[x][y-1].v()=='*';
	if(y<s-1) ret+=p[x][y+1].v()=='*';
	return ret;
}


class campo{
private:
	place** p;
	int size,mine_number,flags,true_flags,discovered;
	int e; //1 ganhou, -1 perdeu;
	int pos[2];
public:
	campo(char diff){
		int i,x,y;
		if(diff==1){
		       size=10;
		       mine_number=10;
		}
		else if(diff==2){
			size=15;
			mine_number=20;
		}else if(diff==3){
			size=20;
			mine_number=30;
		}
		else return;
	        flags=true_flags=0;
		e=0;
		pos[0]=0;
		pos[1]=0;
		discovered=0;
		p=(place**)malloc(sizeof(place*)*size);
		*p=new place[size*size];
		for(i=1;i<size;i++){
			p[i]=p[i-1]+size;
		}
		for(i=0;i<mine_number;i++){
			x=random()%size;
			y=random()%size;
			if(!p[x][y].p() && x+y && cluster(x,y)) p[x][y].set('*');
			else i--;
		}
		for(int j=0;j<size;j++){
			for(i=0;i<size;i++){
				if(!p[i][j].p()){
					p[i][j].set('0'+count(i,j));
				}
			}
		}
	}
	~campo(){
		delete p[0];
		free(p);
	}
	int s(){return size;}
	int end() {return e;}
	bool cluster(int x,int y){
		int val=1;
		if(x>0){
			if(y>0) val+=p[x-1][y-1].v()=='*';
			if(y<size-1) val+=p[x-1][y+1].v()=='*';
			val+=p[x-1][y].v()=='*';
		}
		if(x<size-1){
			if(y>0) val+=p[x+1][y-1].v()=='*';
			if(y<size-1) val+=p[x+1][y+1].v()=='*';
			val+=p[x+1][y].v()=='*';
		}
		if(y>0) val+=p[x][y-1].v()=='*';
		if(y<size-1) val+=p[x][y+1].v()=='*';
		return (size==10&&val<4)||(size==15&&val<6)||(size==20&&val<8);
	}
	void finish(){
		int i,j;
		system("clear");
		for(i=0;i<size;i++){
			printf("|");
			for(j=0;j<size;j++){
				printf(" %c |",p[i][j].v());
			}
			printf("\n-");
			for(j=0;j<size;j++){
				printf("----");
			}
			printf("\n");
		}
	}
	void play(char c){
		if(c=='z'){
			if(p[pos[0]][pos[1]].p()=='.' && flags<mine_number){
			       	true_flags+=p[pos[0]][pos[1]].flag();
				flags++;
			}
			else if(p[pos[0]][pos[1]].p()=='f'){
				true_flags-=p[pos[0]][pos[1]].unflag();
				flags--;
			}
			e=true_flags==mine_number;
		}else if(c==' '){
			if(p[pos[0]][pos[1]].p()=='.'){ 
				e=-p[pos[0]][pos[1]].discover();
				if(p[pos[0]][pos[1]].p()=='0') 
					discovered+=show(pos[0],pos[1]);
				else discovered++;
			}
			if(!e) e=size*size==discovered+mine_number;
		}else if(c=='q'){
			e=2;
		}else{
			switch(c){
				case'A':
					if(pos[0]>0) pos[0]--;
					break;
				case 'B':
					if(pos[0]<size-1)pos[0]++;
					break;
				case 'C':
					if(pos[1]<size-1)pos[1]++;
					break;
				case 'D':
					if(pos[1]>0)pos[1]--;
					break;
			}
		}
	}
	void print(){
		int i,j;
		system("clear");
		for(i=0;i<size;i++){
			printf("|");
			for(j=0;j<size;j++){
				if(i!=pos[0] || j!=pos[1]) printf(" %c |",p[i][j].p());
				else printf("|%c||",p[i][j].p());
			}
			printf("\n-");
			for(j=0;j<size;j++){
				printf("----");
			}
			printf("\n");
		}
		printf("flags left:%d\n",10-flags);
	}
	int count(int x,int y){
		int ret=0;
		if(x>0){
			if(y>0) ret+=p[x-1][y-1].v()=='*';
			if(y<size-1) ret+=p[x-1][y+1].v()=='*';
			ret+=p[x-1][y].v()=='*';
		}
		if(x<size-1){
			if(y>0) ret+=p[x+1][y-1].v()=='*';
			if(y<size-1) ret+=p[x+1][y+1].v()=='*';
			ret+=p[x+1][y].v()=='*';
		}
		if(y>0) ret+=p[x][y-1].v()=='*';
		if(y<size-1) ret+=p[x][y+1].v()=='*';
		return ret;
	}
	int show(int x,int y){
		int ret=1;
		if(x>0){
			if(y>0){
				if(p[x-1][y-1].p()=='.'){
					p[x-1][y-1].discover();
					if(p[x-1][y-1].p()=='0') ret+=show(x-1,y-1);
					else ret++;
				}
			}
			if(y<size-1){
				if(p[x-1][y+1].p()=='.'){
					p[x-1][y+1].discover();
					if(p[x-1][y+1].p()=='0') ret+=show(x-1,y+1);
					else ret++;
				}
			}
			if(p[x-1][y].p()=='.'){
				p[x-1][y].discover();
				if(p[x-1][y].p()=='0') ret+=show(x-1,y);
				else ret++;
			}
		}
		if(x<size-1){
			if(y>0){
				if(p[x+1][y-1].p()=='.'){
					p[x+1][y-1].discover();
					if(p[x+1][y-1].p()=='0') ret+=show(x+1,y-1);
					else ret++;
				}
			}
			if(y<size-1){
				if(p[x+1][y+1].p()=='.'){
					p[x+1][y+1].discover();
					if(p[x+1][y+1].p()=='0') ret+=show(x+1,y+1);
					else ret++;
				}
			}
			if(p[x+1][y].p()=='.'){
				p[x+1][y].discover();
				if(p[x+1][y].p()=='0') ret+=show(x+1,y);
				else ret++;
			}
		}
		if(y>0){
			if(p[x][y-1].p()=='.'){
				p[x][y-1].discover();
				if(p[x][y-1].p()=='0') ret+=show(x,y-1);
				else ret++;
			}
		}
		if(y<size-1){
			if(p[x][y+1].p()=='.'){
				p[x][y+1].discover();
				if(p[x][y+1].p()=='0') ret+=show(x,y+1);
				else ret++;
			}
		}
		return ret;
	}
};

using namespace std;

int input(){
	char c=getchar();
	if(c==27){
		c=getchar();
		c=getchar();
	}
	__fpurge(stdin);
	return c;
}

void play(char diff){
	campo c(diff);
	char e;
	while(!c.end()){
		c.print();
		e=input();
		c.play(e);
	}
	c.finish();
	system("clear");
	if(c.end()==2)return;
	else if(c.end()>0) printf("CONGLATURATIONS!!!!you won!\n");
	else printf("you lost!\n");
	printf("press any key to continue\n");
	getchar();
}

void pre_play(){
	char op=1,in;
	bool flag=true;
	while(flag){
		system("clear");
		printf("Choose your difficulty\n");
		printf("\t%ceasy\n",(op==1)?'>':' ');
		printf("\t%cmedium\n",(op==2)?'>':' ');
		printf("\t%chard\n",(op==3)?'>':' ');
		printf("\t%cback\n",(op==4)?'>':' ');
		in=input();
		if(in=='\n' && op<4)play(op),flag=false;
		else if(in=='\n' && op==4) return;
		else if(in=='A' && op>1) op--;
		else if(in=='B' && op<4) op++;
	}
}

void print_menu(char op){
	system("clear");
	printf("          _____           ____   ___                ___   ___   __    ___   __\n");
	printf("|\\    /|    |    |\\   |  |      |    \\          /  |     |     |  |  |     |  |\n");
	printf("| \\  / |    |    | \\  |  |__    |__   \\        /   |__   |__   |__|  |__   |__|\n");
	printf("|  \\/  |    |    |  \\ |  |         |   \\  /\\  /    |     |     |     |     |  \\\n");
	printf("|      |  __|__  |   \\|  |____  ___|    \\/  \\/     |___  |___  |     |___  |   \\\n");
	printf("\n\n\n");
	printf("\t\t\t%cplay\n",(op==2)?'>':' ');
	printf("\t\t\t%cinstructions\n",(op==1)?'>':' ');
	printf("\t\t\t%cquit\n",(op==0)?'>':' ');
}

void instruction(){
	system("clear");
	printf("\t\tInstructions\n\n\n");
	printf("The rules of the game:\n\n");
	printf("You are searching for mines. When you open a tile it will either be a bomb - which means you died - or a number.\nThe number tells you how many bombs are around that specific tile.\nTo speed up the process, when a 0 tile is opened, it opens all the tiles around it.\nTo win you must discover the position of all mines, and show it by either flagging all the mines or opening all the other tiles.\nEasy enough right? But remmember, to save money you bought as many flags as there are bombs in the field, so don't go wasting them.\n\n");
	printf("The controls:\n\n");
	printf("you move by using the arrow keys.\nTo open a tile, use the spacebar and to flag a mine press z.\nIf you change you mind, and decide that a flagged tile is no longer a mine, press z again\nIf you ever wish to give up, or run out of time to finish you can press q to exit to the main menu\n\n");
	printf("Once you're done looking at this much text, press any key to exit");
	getchar();
}

void main_menu(){
	char op=2,in;
	bool flag=true;
	while(flag){
		print_menu(op);
		in=input();
		if(in=='\n'){
			if(op==2)pre_play();
			else if(op==1) instruction();
			else if(op==0) flag=0;
		}else if(in=='B' && op >0) op--;
		else if(in=='A' && op<2) op++;
	}
}

int main(){
	srandom((double)time(NULL));
	system("stty -icanon -echo");
	main_menu();
	system("stty icanon echo");
	return 0;
}
