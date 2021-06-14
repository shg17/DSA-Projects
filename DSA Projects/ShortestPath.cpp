


#include<bits/stdc++.h>
using namespace std;

template<typename T>
class Graph{
    unordered_map<T, list<pair<T,int> > > m;

public:

    void addEdge(T u,T v,int dist,bool bidir=true){
        m[u].push_back(make_pair(v,dist));
        if(bidir){
            m[v].push_back(make_pair(u,dist));
        }

    }
    void printAdj(){

        //Iterate over all the key value pairs in the map
        for(auto j:m){

            cout<<j.first<<"->";

            //Iterater over the list of cities
            for(auto l: j.second){
                    cout<<"("<<l.first<<","<<l.second<<")";

            }
            cout<<endl;
        }

    }
    void dijsktraSP(T src){

        unordered_map<T,int> dist;

        //Set all distance to infinity
        for(auto j:m){
            dist[j.first] = INT_MAX;
        }

        //Make a set to find a out node with the minimum distance
        set<pair<int, T> > s;

        dist[src] = 0;
        s.insert(make_pair(0,src));

        while(!s.empty()){

            //Find the pair at the front.
            auto p =   *(s.begin());
            T node = p.second;

            int nodeDist = p.first;
            s.erase(s.begin());


            //Iterate over neighbours/children of the current node
            for(auto childPair: m[node]){

                if(nodeDist + childPair.second < dist[childPair.first]){


                    //In the set updation of a particular is not possible
                    // we have to remove the old pair, and insert the new pair to simulation updation
                    T dest = childPair.first;
                    auto f = s.find( make_pair(dist[dest],dest));
                    if(f!=s.end()){
                        s.erase(f);
                    }

                    //Insert the new pair
                    dist[dest] = nodeDist + childPair.second;
                    s.insert(make_pair(dist[dest],dest));

                }

            }
        }
        //Lets print distance to all other node from src


        for(auto d:dist){

            cout<<d.first<<" is located at distance of  "<<d.second<<" from"<<src<<endl;
            cout<<"\n";
        }


    }

};

int main(){

    int noOfPaths;

    Graph<string> india;

    cout<<"Enter no of paths    :" ;
    cin>>noOfPaths;

    string City1,City2;
    int distance;

    while(noOfPaths--)
    {
        cout<<"\n";
        cout<<"Enter City1   :";
        cin>>City1;

        cout<<"\n";

        cout<<"Enter City2   :";
        cin>>City2;

        cout<<"\n";

        cout<<"Enter Distance Between City1 And City 2   :";
        cin>>distance;

        cout<<"----------------------------------------------";

        india.addEdge(City1,City2,distance);
    }

    string src;

    cout<<"\nEnter Source City : ";
    cin>>src;
    cout<<"\n";
    india.dijsktraSP(src);


 return 0;
}
