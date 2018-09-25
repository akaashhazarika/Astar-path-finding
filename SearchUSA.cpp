#include<iostream>
#include<bits/stdc++.h>
using namespace std;
void placeitemsinmap();
map<string,int> mymapper;
map<string,int> cityclosedlist;
int totalcitycount=0;
int totalroads=0;
int totalclosednodes=0;
int routes=0;
int MAXCITY=500;
int MAXROAD=500;
//Custom City data structure for processing
struct custom_place
{
  string name;
  int serialno;
  double latitude;
  double longitude;
};
custom_place* usacities[500];
// Road data structure
struct way
{
  int id;
  custom_place* firstcity;
  custom_place* secondcity;
  double distance;
};
// Custom structure for finding path with parent, cost and currentcity
struct route
{

  double costillnow;
  custom_place *endpoint;
  route *parent;
  int id;
};

custom_place* doescityexists(string name);
void createroadsinmap(string firstcityname, string secondcityname, int weight);
vector<route*> neighbours;
route* currentnode;
vector<route*> finalclosedlist;
route* finalpath;
way* usapaths[500];
custom_place* finaldestination;
deque<route*> openlist;

// Dynamic distance compare
bool dynamiccompare(route* x,route* y)
{
    return (y->costillnow)>(x->costillnow);
}

//Hashmap to return city structure based on city name
custom_place* doescityexists(string name)
{
  if(mymapper.find(name)!=mymapper.end())
    return usacities[mymapper[name]];
  else
    return NULL;
}

//Add cities into memory
void placecityonmap(string cityname, double latitude, double longitude,int totalcity=0)
{
  custom_place* var = new custom_place;
  mymapper[cityname]=totalcitycount;
  var->name=cityname;
  var->longitude = longitude;
  var->latitude = latitude;
  var->serialno=totalcitycount;
  usacities[totalcitycount] = var;
  totalcitycount++;
}
// Add roads into the memory for processing
void createroadsinmap(string firstcityname, string secondcityname, int weight)
{
  custom_place *one = doescityexists(firstcityname);
  custom_place *two = doescityexists(secondcityname);
   if(two==NULL)
    return;

   if(one==NULL)
    return;
  way *road2=new way;
  road2->distance = weight;
  road2->firstcity = two;
  road2->id=totalroads;
  road2->secondcity = one;
  usapaths[totalroads] = road2;
  totalroads++;
  way *road1 = new way;
  road1->distance = weight;
  road1->firstcity = one;
  road1->secondcity = two;
  usapaths[totalroads] = road1;
  road1->id=totalroads;
  totalroads++;
}
//Heuristic function
double straightlinedistance(custom_place* firstcity,custom_place* secondcity)
{
    double pie=3.14159265;
    double part1=pow(69.5*(firstcity->latitude - secondcity->latitude),2);
    double part2=pow((69.5 * cos((firstcity->latitude + secondcity->latitude)/360 * pie)* (firstcity->longitude - secondcity->longitude)),2);
    return sqrt(part1+part2);

}
// Astar distance comparison
bool comparedistanceastar(route* x, route* y) {
  return  (y->costillnow + straightlinedistance(y->endpoint, finaldestination))>(x->costillnow + straightlinedistance(x->endpoint, finaldestination));
}


// Greedy comparison
bool greedycompare(route* x, route* y)
{

    return straightlinedistance(y->endpoint, finaldestination) > straightlinedistance(x->endpoint, finaldestination) ;
}

//Another util function for helping in printing formatted output
void print_util(int type)
{
    if(type==1)
    {
    int counter=0;
    route* a=finalpath;
    while(a!=NULL&&a->parent!=NULL)
    {
        a=a->parent;
        counter++;
    }
    cout<<"Number of Path nodes  "<<counter+1<<endl;
    }
    else if(type==2)
    {
          route* temp=finalpath;
          vector<string> ans;
          while(temp!=NULL&&temp->parent!=NULL)
          {
          ans.push_back(temp->endpoint->name);
          temp=temp->parent;
          }
          reverse(ans.begin(),ans.end());
          for(int i=0;i<ans.size()-1;i++)
          {
          cout<<ans[i]<<", ";
          }
          cout<<finaldestination->name<<endl;

    }
    else
    {

    }

}
// Function to print Answer according to question format
void printformattedans(custom_place* x)
{


      vector<string> mynodes;
      for(int i=0;i<finalclosedlist.size();i++)
      {
          mynodes.push_back(finalclosedlist[i]->endpoint->name);

      }
            cout<<"Expanded Nodes  :";
            for(int i=0;i<mynodes.size()-1;i++)
            cout<<mynodes[i]<<", "<<"";
            cout<<"\nTotal No of Nodes Expanded  : "<<finalclosedlist.size()-1<<endl;
            cout<<"Path: "<<x->name<<", ";
            print_util(2);
            print_util(1);
            cout<<"Total Distance:"<<finalpath->costillnow<<endl;

}
void util_functions(int type)
{
    // Function to find adjacent nodes of a particular node
    if(type==1)
    {
      for (int i=0; i<totalroads; i++)
        {
            way *temp = usapaths[i];
            if (currentnode->endpoint->name == temp->firstcity->name )
                {
                    route *newroute = new route;
                    newroute->costillnow = currentnode->costillnow + temp->distance;
                    newroute->parent = currentnode;
                    newroute->endpoint = temp->secondcity;
                    neighbours.push_back(newroute);
                }
       }
    vector<route*>::iterator im1;
// Below code adds functions to openlist

  for(im1=neighbours.begin(); im1!=neighbours.end(); im1++) {
    // If node is found in closed list do not add to open list
     if(cityclosedlist.find((*im1)->endpoint->name)!=cityclosedlist.end())
        continue;

      deque<route*>:: iterator im2;
    for(im2=openlist.begin();im2!=openlist.end();im2++)
    {
        if((*im2)->endpoint->name==(*im1)->endpoint->name)
        {
            break;
        }
    }

      if(im2==openlist.end())
      {
          // Add node when it is not present in open or closed list
           openlist.push_back(*im1);
      }
      else if((*im1)->costillnow < (*im2)->costillnow)
      {
          // Update node when resulting path is shorter
        openlist.erase(im2);
        openlist.push_back(*im1);

      }
      else
      {

          // DO NOTHING if resulting path is longer
      }
    }
         neighbours.clear();

    }
}

void addtoopen(int type)
{
    // A generic function to arrange elements in openlist based on distance measures
     if(type==1)
    sort(openlist.begin(), openlist.end(), comparedistanceastar);
    else if(type==2)
    {
    sort(openlist.begin(), openlist.end(), dynamiccompare);
    }
    else if(type==3)
    {
    sort(openlist.begin(),openlist.end(),greedycompare);
    }
    else
    {

    }
}

void routecalculator(custom_place* x, custom_place* finalplace,int type=0){
// Put Initial node in queue
  route* starting = new route;
  starting->costillnow = 0;
  starting->parent = NULL;
  starting->endpoint = x;
  starting->id=routes;
  routes++;
  openlist.push_back(starting);
  finaldestination=finalplace;

  while(openlist.size()!=0) {
    // Take out elements from open list and explore its neighbours
    route *presentnode = openlist.front();
    cityclosedlist[presentnode->endpoint->name]=1;
    finalclosedlist.push_back(presentnode);
    openlist.pop_front();

    if (presentnode->endpoint->name == finalplace->name)
     {
    // Final Destination Node is found
        totalclosednodes=finalclosedlist.size();
        finalpath=presentnode;
        printformattedans(x);
        return;


     }
      // Add neighbouring nodes to open list for further expansion
    currentnode=presentnode;
    util_functions(1);

    addtoopen(type);
  }
   cout<<" Route not possible"<<endl;
   exit(0);
}






int main(int argc, char* argv[])
{
placeitemsinmap();
 custom_place* x=doescityexists("minneapolis");
    custom_place* y=doescityexists("cleveland");
    int type=1;
      routecalculator(x,y,type);

 /*
   if(argc!=4)
    {
        cout<<"Incorrect command line execution"<<endl;
        cout<<"Use DistanceMeasure FirstCity Second city"<<endl;
        exit(1);
    }

    custom_place* x=doescityexists(argv[2]);
    custom_place* y=doescityexists(argv[3]);

    int type=0;
    if(strcmp(argv[1],"astar")==0)
        type=1;
    else if(strcmp(argv[1],"dynamic")==0)
        type=2;
    else if(strcmp(argv[1],"greedy")==0)
        type=3;

    else
    {
        cout<<"Incorrect command line execution"<<endl;
        cout<<"Use DistanceMeasure FirstCity Second city"<<endl;

        exit(1);
    }
    if(x!=NULL&&y!=NULL)
    routecalculator(x,y,type);
    else
    cout<<"Cities do not exist in database";
    */

}





void placeitemsinmap()
{
  for(int i=0;i<MAXCITY;i++)
  usacities[i]=NULL;
  placecityonmap("albanyGA", 31.58, 84.17);
  placecityonmap("albanyNY", 42.66, 73.78);
  placecityonmap("albuquerque", 35.11, 106.61);
  placecityonmap("atlanta", 33.76, 84.40);
  placecityonmap("augusta", 33.43, 82.02);
  placecityonmap("austin", 30.30, 97.75);
  placecityonmap("bakersfield", 35.36, 119.03);
  placecityonmap("baltimore", 39.31, 76.62);
  placecityonmap("batonRouge", 30.46, 91.14);
  placecityonmap("beaumont", 30.08, 94.13);
  placecityonmap("boise", 43.61, 116.24);
  placecityonmap("boston", 42.32, 71.09);
  placecityonmap("buffalo", 42.90, 78.85);
  placecityonmap("calgary", 51.00, 114.00);
  placecityonmap("charlotte", 35.21, 80.83);
  placecityonmap("chattanooga", 35.05, 85.27);
  placecityonmap("chicago", 41.84, 87.68);
  placecityonmap("cincinnati", 39.14, 84.50);
  placecityonmap("cleveland", 41.48, 81.67);
  placecityonmap("coloradoSprings", 38.86, 104.79);
  placecityonmap("columbus", 39.99, 82.99);
  placecityonmap("dallas", 32.80, 96.79);
  placecityonmap("dayton", 39.76, 84.20);
  placecityonmap("daytonaBeach", 29.21, 81.04);
  placecityonmap("denver", 39.73, 104.97);
  placecityonmap("desMoines", 41.59, 93.62);
  placecityonmap("elPaso", 31.79, 106.42);
  placecityonmap("eugene", 44.06, 123.11);
  placecityonmap("europe", 48.87, 2.33);
  placecityonmap("ftWorth", 32.74, 97.33);
  placecityonmap("fresno", 36.78, 119.79);
  placecityonmap("grandJunction", 39.08, 108.56);
  placecityonmap("greenBay", 44.51, 88.02);
  placecityonmap("greensboro", 36.08, 79.82);
  placecityonmap("houston", 29.76, 95.38);
  placecityonmap("indianapolis", 39.79, 86.15);
  placecityonmap("jacksonville", 30.32, 81.66);
  placecityonmap("japan", 35.68, 220.23);
  placecityonmap("kansasCity", 39.08, 94.56);
  placecityonmap("keyWest", 24.56, 81.78);
  placecityonmap("lafayette", 30.21, 92.03);
  placecityonmap("lakeCity", 30.19, 82.64);
  placecityonmap("laredo", 27.52, 99.49);
  placecityonmap("lasVegas", 36.19, 115.22);
  placecityonmap("lincoln", 40.81, 96.68);
  placecityonmap("littleRock", 34.74, 92.33);
  placecityonmap("losAngeles", 34.03, 118.17);
  placecityonmap("macon", 32.83, 83.65);
  placecityonmap("medford", 42.33, 122.86);
  placecityonmap("memphis", 35.12, 89.97);
  placecityonmap("mexia", 31.68, 96.48);
  placecityonmap("mexico", 19.40, 99.12);
  placecityonmap("miami", 25.79, 80.22);
  placecityonmap("midland", 43.62, 84.23);
  placecityonmap("milwaukee", 43.05, 87.96);
  placecityonmap("minneapolis", 44.96, 93.27);
  placecityonmap("modesto", 37.66, 120.99);
  placecityonmap("montreal", 45.50, 73.67);
  placecityonmap("nashville", 36.15, 86.76);
  placecityonmap("newHaven", 41.31, 72.92);
  placecityonmap("newOrleans", 29.97, 90.06);
  placecityonmap("newYork", 40.70, 73.92);
  placecityonmap("norfolk", 36.89, 76.26);
  placecityonmap("oakland", 37.80, 122.23);
  placecityonmap("oklahomaCity", 35.48, 97.53);
  placecityonmap("omaha", 41.26, 96.01);
  placecityonmap("orlando", 28.53, 81.38);
  placecityonmap("ottawa", 45.42, 75.69);
  placecityonmap("pensacola", 30.44, 87.21);
  placecityonmap("philadelphia", 40.72, 76.12);
  placecityonmap("phoenix", 33.53, 112.08);
  placecityonmap("pittsburgh", 40.40, 79.84);
  placecityonmap("pointReyes", 38.07, 122.81);
  placecityonmap("portland", 45.52, 122.64);
  placecityonmap("providence", 41.80, 71.36);
  placecityonmap("provo", 40.24, 111.66);
  placecityonmap("raleigh", 35.82, 78.64);
  placecityonmap("redding", 40.58, 122.37);
  placecityonmap("reno", 39.53, 119.82);
  placecityonmap("richmond", 37.54, 77.46);
  placecityonmap("rochester", 43.17, 77.61);
  placecityonmap("sacramento", 38.56, 121.47);
  placecityonmap("salem", 44.93, 123.03);
  placecityonmap("salinas", 36.68, 121.64);
  placecityonmap("saltLakeCity", 40.75, 111.89);
  placecityonmap("sanAntonio", 29.45, 98.51);
  placecityonmap("sanDiego", 32.78, 117.15);
  placecityonmap("sanFrancisco", 37.76, 122.44);
  placecityonmap("sanJose", 37.30, 121.87);
  placecityonmap("sanLuisObispo", 35.27, 120.66);
  placecityonmap("santaFe", 35.67, 105.96);
  placecityonmap("saultSteMarie", 46.49, 84.35);
  placecityonmap("savannah", 32.05, 81.10);
  placecityonmap("seattle", 47.63, 122.33);
  placecityonmap("stLouis", 38.63, 90.24);
  placecityonmap("stamford", 41.07, 73.54);
  placecityonmap("stockton", 37.98, 121.30);
  placecityonmap("tallahassee", 30.45, 84.27);
  placecityonmap("tampa", 27.97, 82.46);
  placecityonmap("thunderBay", 48.38, 89.25);
  placecityonmap("toledo", 41.67, 83.58);
  placecityonmap("toronto", 43.65, 79.38);
  placecityonmap("tucson", 32.21, 110.92);
  placecityonmap("tulsa", 36.13, 95.94);
  placecityonmap("uk1", 51.30, 0.00);
  placecityonmap("uk2", 51.30, 0.00);
  placecityonmap("vancouver", 49.25, 123.10);
  placecityonmap("washington", 38.91, 77.01);
  placecityonmap("westPalmBeach", 26.43, 80.03);
  placecityonmap("wichita", 37.69, 97.34);
  placecityonmap("winnipeg", 49.90, 97.13);
  placecityonmap("yuma", 32.69, 114.62);
     for(int i=0;i<MAXROAD;i++)
    usapaths[i]=NULL;

  createroadsinmap("albanyNY", "montreal", 226);
  createroadsinmap("albanyNY", "boston", 166);
  createroadsinmap("albanyNY", "rochester", 148);
  createroadsinmap("albanyGA", "tallahassee", 120);
  createroadsinmap("albanyGA", "macon", 106);
  createroadsinmap("albuquerque", "elPaso", 267);
  createroadsinmap("albuquerque", "santaFe", 61);
  createroadsinmap("atlanta", "macon", 82);
  createroadsinmap("atlanta", "chattanooga", 117);
  createroadsinmap("augusta", "charlotte", 161);
  createroadsinmap("augusta", "savannah", 131);
  createroadsinmap("austin", "houston", 186);
  createroadsinmap("austin", "sanAntonio", 79);
  createroadsinmap("bakersfield", "losAngeles", 112);
  createroadsinmap("bakersfield", "fresno", 107);
  createroadsinmap("baltimore", "philadelphia", 102);
  createroadsinmap("baltimore", "washington", 45);
  createroadsinmap("batonRouge", "lafayette", 50);
  createroadsinmap("batonRouge", "newOrleans", 80);
  createroadsinmap("beaumont", "houston", 69);
  createroadsinmap("beaumont", "lafayette", 122);
  createroadsinmap("boise", "saltLakeCity", 349);
  createroadsinmap("boise", "portland", 428);
  createroadsinmap("boston", "providence", 51);
  createroadsinmap("buffalo", "toronto", 105);
  createroadsinmap("buffalo", "rochester", 64);
  createroadsinmap("buffalo", "cleveland", 191);
  createroadsinmap("calgary", "vancouver", 605);
  createroadsinmap("calgary", "winnipeg", 829);
  createroadsinmap("charlotte", "greensboro", 91);
  createroadsinmap("chattanooga", "nashville", 129);
  createroadsinmap("chicago", "milwaukee", 90);
  createroadsinmap("chicago", "midland", 279);
  createroadsinmap("cincinnati", "indianapolis", 110);
  createroadsinmap("cincinnati", "dayton", 56);
  createroadsinmap("cleveland", "pittsburgh", 157);
  createroadsinmap("cleveland", "columbus", 142);
  createroadsinmap("coloradoSprings", "denver", 70);
  createroadsinmap("coloradoSprings", "santaFe", 316);
  createroadsinmap("columbus", "dayton", 72);
  createroadsinmap("dallas", "denver", 792);
  createroadsinmap("dallas", "mexia", 83);
  createroadsinmap("daytonaBeach", "jacksonville", 92);
  createroadsinmap("daytonaBeach", "orlando", 54);
  createroadsinmap("denver", "wichita", 523);
  createroadsinmap("denver", "grandJunction", 246);
  createroadsinmap("desMoines", "omaha", 135);
  createroadsinmap("desMoines", "minneapolis", 246);
  createroadsinmap("elPaso", "sanAntonio", 580);
  createroadsinmap("elPaso", "tucson", 320);
  createroadsinmap("eugene", "salem", 63);
  createroadsinmap("eugene", "medford", 165);
  createroadsinmap("europe", "philadelphia", 3939);
  createroadsinmap("ftWorth", "oklahomaCity", 209);
  createroadsinmap("fresno", "modesto", 109);
  createroadsinmap("grandJunction", "provo", 220);
  createroadsinmap("greenBay", "minneapolis", 304);
  createroadsinmap("greenBay", "milwaukee", 117);
  createroadsinmap("greensboro", "raleigh", 74);
  createroadsinmap("houston", "mexia", 165);
  createroadsinmap("indianapolis", "stLouis", 246);
  createroadsinmap("jacksonville", "savannah", 140);
  createroadsinmap("jacksonville", "lakeCity", 113);
  createroadsinmap("japan", "pointReyes", 5131);
  createroadsinmap("japan", "sanLuisObispo", 5451);
  createroadsinmap("kansasCity", "tulsa", 249);
  createroadsinmap("kansasCity", "stLouis", 256);
  createroadsinmap("kansasCity", "wichita", 190);
  createroadsinmap("keyWest", "tampa", 446);
  createroadsinmap("lakeCity", "tampa", 169);
  createroadsinmap("lakeCity", "tallahassee", 104);
  createroadsinmap("laredo", "sanAntonio", 154);
  createroadsinmap("laredo", "mexico", 741);
  createroadsinmap("lasVegas", "losAngeles", 275);
  createroadsinmap("lasVegas", "saltLakeCity", 486);
  createroadsinmap("lincoln", "wichita", 277);
  createroadsinmap("lincoln", "omaha", 58);
  createroadsinmap("littleRock", "memphis", 137);
  createroadsinmap("littleRock", "tulsa", 276);
  createroadsinmap("losAngeles", "sanDiego", 124);
  createroadsinmap("losAngeles", "sanLuisObispo", 182);
  createroadsinmap("medford", "redding", 150);
  createroadsinmap("memphis", "nashville", 210);
  createroadsinmap("miami", "westPalmBeach", 67);
  createroadsinmap("midland", "toledo", 82);
  createroadsinmap("minneapolis", "winnipeg", 463);
  createroadsinmap("modesto", "stockton", 29);
  createroadsinmap("montreal", "ottawa", 132);
  createroadsinmap("newHaven", "providence", 110);
  createroadsinmap("newHaven", "stamford", 92);
  createroadsinmap("newOrleans", "pensacola", 268);
  createroadsinmap("newYork", "philadelphia", 101);
  createroadsinmap("norfolk", "richmond", 92);
  createroadsinmap("norfolk", "raleigh", 174);
  createroadsinmap("oakland", "sanFrancisco", 8);
  createroadsinmap("oakland", "sanJose", 42);
  createroadsinmap("oklahomaCity", "tulsa", 105);
  createroadsinmap("orlando", "westPalmBeach", 168);
  createroadsinmap("orlando", "tampa", 84);
  createroadsinmap("ottawa", "toronto", 269);
  createroadsinmap("pensacola", "tallahassee", 120);
  createroadsinmap("philadelphia", "pittsburgh", 319);
  createroadsinmap("philadelphia", "newYork", 101);
  createroadsinmap("philadelphia", "uk1", 3548);
  createroadsinmap("philadelphia", "uk2", 3548);
  createroadsinmap("phoenix", "tucson", 117);
  createroadsinmap("phoenix", "yuma", 178);
  createroadsinmap("pointReyes", "redding", 215);
  createroadsinmap("pointReyes", "sacramento", 115);
  createroadsinmap("portland", "seattle", 174);
  createroadsinmap("portland", "salem", 47);
  createroadsinmap("reno", "saltLakeCity", 520);
  createroadsinmap("reno", "sacramento", 133);
  createroadsinmap("richmond", "washington", 105);
  createroadsinmap("sacramento", "sanFrancisco", 95);
  createroadsinmap("sacramento", "stockton", 51);
  createroadsinmap("salinas", "sanJose", 31);
  createroadsinmap("salinas", "sanLuisObispo", 137);
  createroadsinmap("sanDiego", "yuma", 172);
  createroadsinmap("saultSteMarie", "thunderBay", 442);
  createroadsinmap("saultSteMarie", "toronto", 436);
  createroadsinmap("seattle", "vancouver", 115);
  createroadsinmap("thunderBay", "winnipeg", 440);
}
