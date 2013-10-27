// blatt2.C (Read digraph from file and find strongly connected components by non-recursive algorithm)
// Algorithmus arbeitet analog wie in Vorlesung, es wurde nur die rekursive DFS durch iterative DFS ersetzt.
#include "graph.h"
using namespace std;

//return NodeId für nicht besuchten Nachbar entlang Kantenrichtung, return num_nodes(), falls kein Nachbar vorhanden
Graph::NodeId nextw(Graph g, Graph::NodeId v, int visited[]);
//return NodeId für nicht besuchten Nachbar entgegen Kantenrichtung, return num_nodes(), falls kein Nachbar vorhanden
Graph::NodeId prevw(Graph g, Graph::NodeId v, int visited[]);

Graph::NodeId nextw(Graph g, Graph::NodeId v, int visited[]){
	vector<Graph::NodeId> S;

	for (Graph::EdgeId e = 0; e != g.get_node(v).out_edges().size(); ++e) 
    {
    	S.push_back(g.get_edge(g.get_node(v).out_edges()[e]).get_head());
    }

	while(!S.empty()){
		if(visited[S.back()]==0){
			return S.back();
		} else {
			S.pop_back();
		}
	}

	return g.num_nodes();
}

Graph::NodeId prevw(Graph g, Graph::NodeId v, int visited[]){
	vector<Graph::NodeId> S;

	for (Graph::EdgeId e = 0; e != g.get_node(v).in_edges().size(); ++e) 
    {
    	S.push_back(g.get_edge(g.get_node(v).in_edges()[e]).get_tail());
    }

	while(!S.empty()){
		if(visited[S.back()]==0){
			return S.back();
		} else {
			S.pop_back();
		}
	}

	return g.num_nodes();
}


int main(int argc, char* argv[])
{
	  Graph g;
      if (argc > 1) 
      {
            g = read_graph(argv[1]);
      }

/*
fs: Stack, auf dem Knoten in markierter Reihenfolge abgelegt werden
visited: 0, wenn noch nicht besucht, -1 falls evtl. nochmal besucht werden muss,
z.B. wenn noch nicht alle Nachbarn markiert wurden, 1 falls fertig und markiert
comp: Zuordnung des Knoten zu starke Zshgskomp.
*/

      vector<Graph::NodeId> fs;
      int visited[g.num_nodes()];
      int found[g.num_nodes()];
      int comp[g.num_nodes()];

//Initialisieren
      for (Graph::NodeId v = 0; v != g.num_nodes(); ++v)
      {
      	visited[v]=0;
      	found[v]=0;
      	comp[v]=0;
      }

//Nutze Stack, um Backtracking von rekursivem DFS zu simulieren (hinterster Knoten ist
//immer der zuletzt besuchte

      vector<Graph::NodeId> S; //Stack

      for (Graph::NodeId v = 0; v != g.num_nodes(); ++v)
      {
      	if (visited[v]==0)	//falls noch nicht besucht
      	{
      		visited[v]=-1;	//muss evtl. nochmal besucht werden
      		S.push_back(v); //akt. Knoten auf Stack

      		while (!S.empty()) //arbeite Stack ab
      		{
      			Graph::NodeId u=nextw(g,v,visited); 
      			if(u!=g.num_nodes())	//falls nächster Nachbar vorhanden
      			{
      				visited[u]=-1;		//Nachbar muss evtl. nochmal besucht werden
      				if(nextw(g,u,visited)!=g.num_nodes())
      				{
      					S.push_back(u);		//Nachbar auf Stack, falls er auch Nachbarn hat
      				}
      				v=u;	//gehe eine "Ebene" tiefer (Nachbar ist jetzt aktueller Knoten)
      			}
      			else	//falls kein Nachbar vorhanden
      			{
      				visited[v]=1;	//sind fertig mit akt. Knoten

      				if(found[v]==0)	//falls nicht schon schon vorher markiert
      				{
      					found[v]=1;
      					fs.push_back(v);	//auf den markierte Knoten Stack
      				}
      				if (!S.empty()) {		//falls Stack noch nicht leer
      					v=S.back();			//hole nächsten Knoten
      					S.pop_back();
      				}
      				if (visited[v]==-1){	//falls Knoten nochmal besucht werden muss
      					S.push_back(v);		//wieder auf Stack
      				}
      			}


      		}
      	}
      }

      //zurücksetzen
      S.clear();

      for (Graph::NodeId v = 0; v != g.num_nodes(); ++v)
      {
      	visited[v]=0;
      }

      int k=0;      

      //Analog wie oben, nur diesmal werden Nummern rückwärts abgearbeitet
      while (!fs.empty())
      {
      	Graph::NodeId v=fs.back();
      	fs.pop_back();
      	
      	if (visited[v]==0)
      	{
      		k++;
      		visited[v]=-1;
      		S.push_back(v);

      		while (!S.empty())
      		{
      			Graph::NodeId u=prevw(g,v,visited);
      			if(u!=g.num_nodes())
      			{
      				visited[u]=-1;
      				if(prevw(g,u,visited)!=g.num_nodes())
      				{
      					S.push_back(u);
      				}
      				v=u;
      			}
      			else
      			{
      				visited[v]=1;
      				if(comp[v]==0)	//weise Knoten Zhgskomp. zu
      				{
      					comp[v]=k;
      				}
      				if (!S.empty())
      				{
      					v=S.back();
      					S.pop_back();
      				}
      				if (visited[v]==-1)
      				{
      					S.push_back(v);
      				}
      			}


      		}
      	}      	
      }

//Ausgabe
     int numcomp=0;
     for (int i=0;i!=g.num_nodes();++i)
      {
      	if(comp[i]>numcomp)
      		numcomp=comp[i];
      }

      cout << numcomp << "\n";

      for (int i=1;i<=numcomp;++i)
      {
      	for (int j=0;j!=g.num_nodes();++j)
      	{
      		if(comp[j]==i)
      			cout << j << " ";
      	}
      	cout << "\n";
      }

return 0;
}
