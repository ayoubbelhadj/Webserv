#include "../server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;

/*server_class*/
void	server::portSetter(string prt){
	port = prt;
}

void	server::set_slistener(int s){
	_s_listener = s;
}

void	server::set_sconnection(int s){
	_s_connection = s;
}

string	server::portGetter(){
	return port;
}

int	server::get_slistener(){
	return _s_listener;
}

int	server::get_sconncetion(){
	return _s_connection;
}

void	server::set_ip(string ip){
	this->ip  = ip;
}

string	server::get_ip(){
	return ip;
}

void	server::set_isdefault(bool b){
	is_default = b;
}

bool	server::get_isdefault(){
	return is_default;
}

void	server::set_my_default(int index){
	my_default = index;
}

int		server::get_my_default(){
	return my_default;
}

/*serersInfos_class*/
serversInfos::serversInfos(const vector<server>& servers){
	this->servers = servers;
}

void	serversInfos::closeListeners(){
	vector<server>::iterator it;
	for (it = servers.begin(); it < servers.end();it++)
		close(it->get_slistener());
}

vector<server> serversInfos::get_servers(){
	return servers;
}

void	server::set_request(string rq){
	request = rq;
}

void	server::set_response(string rs){
	response = rs;
}

string	server::get_request(){
	return request;
}

string	server::get_response(){
	return response;
}

void serversInfos::SetListener(){
	vector<server>::iterator it;
	int reusePortOption = 1;
	for(it = servers.begin();it < servers.end();it++){
		struct addrinfo hints, *res;
		bzero(&hints, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;

		if (getaddrinfo(it->getIp().c_str(), (it->portGetter()).c_str(),
			&hints, &res) != 0){
			perror("getaddrinfo");
			exit(EXIT_FAILURE);
		}
		
		if (!it->get_isdefault()){
			it->set_slistener(servers[it->get_my_default()].get_slistener());
			freeaddrinfo(res);
		}
		else {
			it->set_slistener(socket(res->ai_family,
				res->ai_socktype, res->ai_protocol));
			
			setsockopt(it->get_slistener(), SOL_SOCKET,
				SO_REUSEPORT, &reusePortOption, sizeof(reusePortOption));
			
			allSockets.push_back(it->get_slistener());//pushtoallsifd
			it->mysockets.push_back(it->get_slistener());

			fcntl(it->get_slistener(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
			if (bind(it->get_slistener(), res->ai_addr,res->ai_addrlen) < 0){
				perror("bind");
				freeaddrinfo(res);
				closeListeners();
				exit(EXIT_FAILURE);
			}
			freeaddrinfo(res);
			if (listen(it->get_slistener(), SOMAXCONN) < 0)
			{
				cout << RED << "listen() failed" << RESET_TEXT << endl;
				closeListeners();
				exit(EXIT_FAILURE);
			}
		}
		cout << "ip: " << it->getIp();
		cout << " ,port: " << it->portGetter();
		cout << " ,listening on socket: "
			<< it->get_slistener() << endl;
	}
}

