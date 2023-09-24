#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <numeric>
#include <map>

using namespace std;

ifstream fin, fin2;// fin2 for anime input and fin for user input
ofstream fout, fout2;

int num_of_user = 4;
int num_of_anime = 5;

class admin;
class security
{
protected:
    virtual void display() = 0;

};
class anime
{
    friend class admin;
protected:
    string name, description, genre;
    string year, episode;
    vector<int> rating;
public:
    int total_watched;
    vector<string> comments;
    anime()
    {
        total_watched = 0;
    }
    anime (string name, string description, string genre, string year, string episode) : name(name), description(description), genre(genre), year(year)
    {
        total_watched= 0;
        this->episode = episode;
    }
    void display()
    {
        fout2 << "Anime: " << name << endl;
        //fout2 << description << endl;
        fout2 << "Genre: " << genre << endl;
        fout2 << "Released on " << year << endl;
        fout2 << "Episodes: " << episode << endl;
    }
    double average_rating()
    {
        return ( 1.0 * accumulate(rating.begin(), rating.end(), 0) / ( rating.size() * 1.00 ) );
    }
    friend class user;
    friend void show_comments(anime& A);
    friend void show_top_anime(vector<anime>& Anime);
    bool operator==(const anime &lol)
    {
        if(name == lol.name) return true;
        else return false;
    }
};
class user : public security
{
    friend class admin;
    string id, pass;
    vector<anime> watched;
public:
    void display()
    {
        fout << id + " " + pass + "\n";
    }
    user() {}
    user(string id, string pass) : id(id), pass(pass) {}
    void addWatch(anime& A)
    {
        auto it = find(watched.begin(), watched.end(), A);
        if(it == watched.end())
        {
            watched.push_back(A);
            A.total_watched++;
            cout << A.name << " has been added\n";
        }
        else cout << "Already in list\n";
    }
    void add_rating(anime& A, int rate)
    {
        auto it = find(watched.begin(), watched.end(), A);
        if( it != watched.end())
        {
            it->rating.push_back(rate);
        }
        else cout << "User: " + id + " has not watched the anime: " + A.name + '\n';
    }
    void add_comment(anime &a, vector<anime>&Anime)
    {
        auto it = find(watched.begin(), watched.end(), a);
        if( it != watched.end())
        {
            cout << "Enter your comment for " + a.name + ": ";
            string temp;
            getline(cin, temp);
            auto it2 = find(Anime.begin(), Anime.end(), a);
            it2->comments.push_back(temp);
        }
        else cout << "User: " + id + " has not watched the anime: " + a.name + '\n';
    }
    void watched_anime()
    {
        cout << "---User: " << id << " watchlist is: ---\n";
        for(int i = 0; i < watched.size(); ++i)
        {
            cout << 1+i << ". " << watched[i].name + "\n";
        }

    }
    bool operator==(const user &lol)
    {
        if(id == lol.id and pass == lol.pass) return true;
        else return false;
    }
};
class admin : public security
{
    string id, pass;
public:
    void display()
    {
        cout << "Admin id: " + id + '\n';
    }
    admin(string id, string pass) : id(id), pass(pass) {}
    bool operator==(const admin &lol)
    {
        if(id == lol.id and pass == lol.pass) return true;
        else return false;
    }
    void delete_user(user& todelete, vector<user>& userList);
    void delete_anime(anime& todelete, vector<anime>& Anime);
    void add_user(user& toadd, vector<user>& userList);
    void add_anime(anime& toadd, vector<anime>& Anime);
};
void admin :: delete_user(user& todelete, vector<user>& userList)
{
    auto it = find(userList.begin(), userList.end(), todelete);
    if(it != userList.end())
    {
        --num_of_user;
        cout << "User ID: " << todelete.id << " has been deleted\n";
        userList.erase(it);
    }
    else
        cout << "User not found\n";
}
void admin :: add_user(user& toadd, vector<user>& userList)
{
    auto it = find(userList.begin(), userList.end(), toadd);
    if(it == userList.end())
    {
        userList.push_back(toadd);
        cout << "User ID: " << toadd.id << " has been added\n";
        num_of_user++;
    }
    else cout << "Already in the list\n";
}
void admin :: add_anime(anime& toadd, vector<anime>& Anime)
{
    auto it = find(Anime.begin(), Anime.end(), toadd);
    if(it == Anime.end())
    {
        Anime.push_back(toadd);
        cout << "The anime: " << toadd.name << " has been added\n";
        num_of_anime++;
    }
    else cout << "Anime already exists\n";
}
void admin :: delete_anime(anime& todelete, vector<anime>& Anime)
{
    auto it = find(Anime.begin(), Anime.end(), todelete);
    if(it != Anime.end())
    {
        cout << (*it).name +" has been deletd\n";
        Anime.erase(it);
        --num_of_anime;
    }
    else cout << "Not found\n";
}
template<typename T>
void show(T& obj)
{
    obj.display();
}
void show_comments(anime& A, vector<anime>& Anime)
{
    auto it = find(Anime.begin(), Anime.end(), A );
    if( it != Anime.end() )
    {
        cout << "Watched by :" << it->total_watched << " users\n";
        for(int i = 0; i < it->comments.size(); ++i)
        {
            cout << i+1 << ". " + it->comments[i] + '\n';
        }
    }
    else cout << "Anime not in list\n";
}

//class viewer :
int main()
{

    admin Admin("2107062", "CSE1206");
    vector<user> userList;
    vector<anime> Anime;
    fin.open("input_2107062.txt");
    fin2.open("input_2107062_2.txt");
    fout.open("output_2107062.txt");
    fout2.open("output_2107062_2.txt");
Load_Data:
    {

        for(int i = 0; i < num_of_user; ++i)
        {
            string a, b;
            fin >> a >> b;
            userList.push_back(user(a, b));
        }
        for(int i = 0; i < num_of_anime; ++i)
        {
            string a, b, c, d, e;
            getline(fin2, a);
            getline(fin2, b);
            getline(fin2, c);
            getline(fin2, d);
            getline(fin2, e);
            anime temp(a, b, c, d, e);
            Anime.push_back(temp);
            userList[0].addWatch(temp);
        }

    }
    int t = INT_MAX;
    while(t)
    {
        cout << "1. User registration\n";
        cout << "2. User login\n";
        cout << "3. Admin login\n";
        cout << "4. Exit\n";
        cin >> t;
        if(t == 1)
        {
            cout << "Enter id and pass: \n";
            string a, b;
            cin >> a >> b;
            user temp(a, b);
            if(find(userList.begin(), userList.end(), temp) == userList.end())
                userList.push_back(temp);
            else cout << "User exits\n";
        }
        else if(t == 2)
        {
            cout << "Enter id and pass: \n";
            string a, b;
            cin >> a >> b;
            user temp(a, b);
            auto it = find(userList.begin(), userList.end(), temp);
            if(it != userList.end())
            {
                cout << "User: ";
                it->display();
                cout << " logged in\n-----\n";
                int p = INT_MAX;
                while(p)
                {
                    cout << "0. Log Out\n";
                    cout << "1. Add anime to list\n2. Add rating to an Anime\n";
                    cout << "3. Add comment to an Anime\n4. Show all watched animes\n";
                    cout << "5. Show certain anime comments\n";
                    cin >> p;
                    if(p == 1)
                    {
                        cout << "Enter anime name to add: \n";
                        string name;
                        cin >> ws;
                        getline(cin, name);
                        anime temp(name, "", "", "", "");
                        it->addWatch(temp);
                    }
                    else if ( p == 5)
                    {
                        cout << "Enter anime name to show the comments: \n";
                        string name;
                        cin >> ws;
                        getline(cin, name);
                        anime temp(name, "", "", "", "");
                        show_comments(temp, Anime);
                    }
                    else if ( p == 0) break;
                    else if ( p == 2)
                    {
                        cout << "Enter anime name to rate: \n";
                        cin >> ws;
                        string s;
                        getline(cin, s);
                        anime temp(s, "", "", "", "");
                        cout << "Enter rating: \n";
                        int a;
                        cin >> a;
                        (*it).add_rating(temp, a);
                    }
                    else if ( p == 3)
                    {
                        cout << "Enter anime name to comment: \n";
                        cin >> ws;
                        string s;
                        getline(cin, s);
                        anime temp(s, "", "", "", "");
                        it->add_comment(temp, Anime);
                    }
                    else if ( p == 4)
                    {
                        it->watched_anime();
                    }
                    else cout << "Wrong choice";

                }
                cout << "-----\n";
            }
            cout << "Wrong id password\n";
        }
        else if (t == 3)
        {
            cout << "Enter id , pass: \n";
            string id, pass;
            cin >> id >> pass;
            admin temp(id, pass);
            if(temp == Admin)
            {
                cout << "---";
                show(Admin);
                int pq = -99999;
                while(pq)
                {
                    cout << "0. LogOut\n1. Add anime\n2. Add User\n3. delete anime\n4. delete user\n";
                    cin >> pq;
                    if(pq == 0) break;
                    else if( pq == 1)
                    {
                        cout << "Enter anime name: ";
                        string name, description, genre, year, episode;
                        cin >> ws;
                        getline(cin, name);
                        getline(cin, description);
                        getline(cin, genre);
                        cin >> year >> episode;
                        anime temp(name, description, genre, year, episode);
                        Admin.add_anime(temp, Anime);
                    }
                    else if (pq == 2)
                    {
                        cout << "Enter user id, pass: \n";
                        string id, pass;
                        cin >> id >> pass;
                        user temp(id, pass);
                        Admin.add_user(temp, userList);
                    }
                    else if (pq == 3)
                    {
                        cout << "Enter anime name to delete: \n";
                        string s;
                        cin >> ws;
                        getline(cin, s);
                        anime temp(s, "1", "1", "1", "1");
                        Admin.delete_anime(temp, Anime);
                    }
                    else if (pq == 4)
                    {
                        cout << "Enter user id, pass to delete : \n";
                        string id, pass;
                        cin >> id >> pass;
                        user temp(id, pass);
                        Admin.delete_user(temp, userList);
                    }
                    else cout << "wrong selection\n";
                }
            }
            else cout << "Wrong\n";
        }
        else if (t == 4) break;
        else cout << "Wrong choice\n";
    }
show:
    {
        for(int i = 0; i < num_of_user; ++i)
        {
            show(userList[i]);
        }
        for(int i = 0; i < num_of_anime; ++i)
        {
            show(Anime[i]);
        }
    }
    fin.close(), fin2.close(), fout.close(), fout2.close();
    return 0;
}

