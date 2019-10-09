#include"Simplex.h"
#include<cassert>
#include"../core/Constant.h"
int SimplexAI::id[21 << 1];
double SimplexAI::a[21][21];
map<State, double> SimplexAI::avgScore;

vector<Movement> SimplexAI::getMovementForMarisa(const State& state)
{
	vector<Movement> res;
	Movement movement;
	for (int i = 0; i <= 4; ++i)
	{
		if (i == 4)
		{
			res.push_back(Movement::createMovementStay());
			break;
		}
		movement.direction = i;
		Vec2i newPos = state.marisa.position + cns::delta[i];
		if (cns::outofRange(newPos, startState.map.size()))
			continue;
		if (startState.map[newPos.x][newPos.y].type == GridType::Wall)
			continue;
		if (startState.map[newPos.x][newPos.y].type == GridType::Pile)
		{
			if (!state.marisa.have)
			{
				movement.type = MovementType::Get;
				res.push_back(movement);
			}
			continue;
		}
		if (startState.map[newPos.x][newPos.y].type == GridType::Shrine)
		{
			if (state.marisa.have)
			{
				movement.type = MovementType::Put;
				res.push_back(movement);
			}
			continue;
		}
		//�ж������Ļ���
		int64_t colorUp = 0;
		if (state.minoriko.hp > 0)
		{
			if (startState.map[state.minoriko.position.x][state.minoriko.position.y].type == GridType::Trigger)
				colorUp |= startState.map[state.minoriko.position.x][state.minoriko.position.y].tag;
		}
		if (startState.map[state.marisa.position.x][state.marisa.position.y].type == GridType::Trigger)
			colorUp |= startState.map[state.marisa.position.x][state.marisa.position.y].tag;
		if (startState.map[newPos.x][newPos.y].type == GridType::MoveableWall && (startState.map[newPos.x][newPos.y].tag & colorUp))
			continue;
		movement.type = MovementType::Move;
		res.push_back(movement);
	}
	if (state.marisa.have)
	{
		//�Ժ���
		if (state.marisa.hp < startState.marisaMaxHp)
		{
			movement.type = MovementType::Eat;
			res.push_back(movement);
		}
		//Ͷ������
		for (int i = 0; i <= 4; ++i)
		{
			Vec2i newPos = state.minoriko.position + cns::delta[i];
			bool ok = false;
			for (int j = 0; j < 4; ++j)
			{
				for (int k = 1; k <= 2; ++k)
				{
					Vec2i tarPos = state.marisa.position + cns::delta[j] * k;
					if (newPos == tarPos)
					{
						ok = true;
						movement.type = MovementType::Throw;
						movement.direction = j;
						movement.distance = k;
						res.push_back(movement);
						break;
					}
				}
				if (ok)break;
			}
		}
	}
	return res;
}
vector<Movement> SimplexAI::getMovementForMinoriko(const State& state)
{
	vector<Movement> res;
	Movement movement = Movement::createMovementStay();
	if (state.minoriko.hp <= 0)
	{
		res.push_back(movement);
		return res;
	}
	for (int i = 0; i <= 4; ++i)
	{
		if (i == 4)
		{
			res.push_back(Movement::createMovementStay());
			continue;
		}
		movement.direction = i;
		Vec2i newPos = state.minoriko.position + cns::delta[i];
		if (cns::outofRange(newPos, startState.map.size()))
			continue;
		if (startState.map[newPos.x][newPos.y].type == GridType::Wall)
			continue;
		if (startState.map[newPos.x][newPos.y].type == GridType::Pile)
		{
			if (!state.minoriko.have)
			{
				movement.type = MovementType::Get;
				res.push_back(movement);
			}
			continue;
		}
		if (startState.map[newPos.x][newPos.y].type == GridType::Shrine)
		{
			if (state.minoriko.have)
			{
				movement.type = MovementType::Put;
				res.push_back(movement);
			}
			continue;
		}
		//�ж������Ļ���
		int64_t colorUp = 0;
		if (state.minoriko.hp > 0)
		{
			if (startState.map[state.minoriko.position.x][state.minoriko.position.y].type == GridType::Trigger)
				colorUp |= startState.map[state.minoriko.position.x][state.minoriko.position.y].tag;
		}
		if (startState.map[state.marisa.position.x][state.marisa.position.y].type == GridType::Trigger)
			colorUp |= startState.map[state.marisa.position.x][state.marisa.position.y].tag;
		if (startState.map[newPos.x][newPos.y].type == GridType::MoveableWall && (startState.map[newPos.x][newPos.y].tag & colorUp))
			continue;
		movement.type = MovementType::Move;
		assert(movement.direction >= 0 && movement.direction < 4);
		res.push_back(movement);
	}
	if (state.minoriko.have)
	{
		//�Ժ���
		if (state.minoriko.hp < startState.minorikoMaxHp)
		{
			movement.type = MovementType::Eat;
			res.push_back(movement);
		}
		//Ͷ������
		for (int i = 0; i <= 4; ++i)
		{
			Vec2i newPos = state.marisa.position + cns::delta[i];
			bool ok = false;
			for (int j = 0; j < 4; ++j)
			{
				for (int k = 1; k <= 2; ++k)
				{
					Vec2i tarPos = state.minoriko.position + cns::delta[j] * k;
					if (newPos == tarPos)
					{
						ok = true;
						movement.type = MovementType::Throw;
						movement.direction = j;
						movement.distance = k;
						res.push_back(movement);
						break;
					}
				}
				if (ok)break;
			}
		}
	}
	return res;
}
void SimplexAI::pivot(int l, int e, int row, int col)
{
	swap(id[e], id[row + l]);
	double r = a[l][e]; a[l][e] = 1.0;
	for (int j = 0; j <= row; ++j)
		a[l][j] /= r;
	for (int i = 0; i <= col; ++i)
		if (i != l) {
			r = a[i][e]; a[i][e] = 0;
			for (int j = 0; j <= row; ++j)
				a[i][j] -= r * a[l][j];
		}
}
double SimplexAI::solve(State state)
{
	if (avgScore.find(state) != avgScore.end())return avgScore[state];
	static int c = 0;
	c++;
	if (c % 100000 == 0)cout << c << endl;
	if (state.marisa.hp <= 0 || state.roundLeft <= 0)
		return avgScore[state] = 1.0+((double)rand() / 100000000000000.0)- (16383.0 / 100000000000000.0);
	double p[10][10];
	memset(p, 0, sizeof(p));
	vector<Movement>MovementMarisa = getMovementForMarisa(state);
	vector<Movement>MovementMinoriko = getMovementForMinoriko(state);
	int szmarisa = MovementMarisa.size(), szminoriko = MovementMinoriko.size();
	int row = szmarisa, col = szminoriko;
	//�����������
	for (int i = 0; i < szmarisa; ++i)
		for (int j = 0; j < szminoriko; ++j)
		{
			Game game;
			game.newGame(startState);
			game.score = 0;
			game.marisa = state.marisa;
			game.minoriko = state.minoriko;
			game.round = game.roundLimit - state.roundLeft;
			game.setMovementForMarisa(MovementMarisa[i]);
			game.setMovementForMinoriko(MovementMinoriko[j]);
			game.roundFinish();
			State nextState;
			nextState.marisa = game.marisa;
			nextState.minoriko = game.minoriko;
			nextState.roundLeft = game.roundLimit - game.round;
			p[i][j] = solve(nextState) + game.score;
		}
	/*
	for (int i = 0; i < szmarisa; ++i)
	{
		for (int j = 0; j < szminoriko; ++j)
		{
			printf("%lf ", p[i][j]);
		}
		cout << endl;
	}*/
	//������ 

	memset(a, 0, sizeof(a));
	memset(id, 0, sizeof(id));
	for (int j = 1; j <= row; ++j)
		a[0][j] = -1.0, id[j] = j;
	for (int i = 1; i <= col; ++i) {
		for (int j = 1; j <= row; ++j)
			a[i][j] = -p[j - 1][i - 1];
		a[i][0] = -1.0;
	}
	int i, j, l, e; double k;
	while (true)
	{
		while (true)
		{
			l = e = 0; k = -cns::eps;
			for (i = 1; i <= col; ++i)
				if (a[i][0] < k)
				{
					k = a[i][0];
					l = i;
				}
			if (!l) break;
			for (j = 1; j <= row && !e; ++j)
				if (a[l][j] < -cns::eps)
					e = j;				
			assert(e);
			pivot(l, e, row, col);
		}
		try
		{
			long long ct = 0;
			while (true) {
				for (j = 1; j <= row; ++j)
					if (a[0][j] > cns::eps)
						break;
				if ((e = j) > row) break;
				ct++;
				if (ct > 100000000)
				{
					throw(-1);
					cout << e << endl;
				}
				k = 1e18; l = 0;
				for (i = 1; i <= col; ++i)
					if (a[i][e] > cns::eps && (a[i][0] / a[i][e]) < k)
					{
						k = a[i][0] / a[i][e];
						l = i;
					}
				assert(l);
				pivot(l, e, row, col);
			}
		}
		catch (int t)
		{
			for (int i = 0; i < szmarisa; ++i)
			{
				for (int j = 0; j < szminoriko; ++j)
				{
					printf("%lf ", p[i][j]);
				}
				cout << endl;
			}
			//cout << "bad" << endl;
			continue;
		}
		break;
	}
	double res[21 << 1];
	memset(res, 0, sizeof(res));
	for (i = 1; i <= col; ++i)res[id[row + i]] = a[i][0];
	double sum = 0.0;
	for (i = 1; i <= row; ++i)sum += res[i];
	sum = 1.0 / sum;
	return avgScore[state] = sum;
}
void SimplexAI::init(GameInfo info)
{
	startState = info;
	State sta;
	sta.marisa = info.marisa;
	sta.minoriko = info.minoriko;
	sta.roundLeft = info.roundLimit - info.round;
	solve(sta);
}
const char gridStr[10] = { ' ','*','S','#','.','@','_' };
void SimplexAI::printInfo(GameInfo& info)
{
	cout << (info.who ? "����ħ��ɳ" : "�����") << endl;
	cout << "��" << info.round << "�룬��ʣ��" << info.roundLimit - info.round << "��" << endl;
	cout << "�Ѱ���:" << info.score << endl;
	cout << "HP: ���" << info.minoriko.hp << "/" << info.minorikoMaxHp << " ħ��ɳ" << info.marisa.hp << "/" << info.marisaMaxHp << endl;
	int64_t colorUp = 0;
	if (info.minoriko.hp>0)
		if (info.map[info.minoriko.position.x][info.minoriko.position.y].type == GridType::Trigger)
			colorUp |= info.map[info.minoriko.position.x][info.minoriko.position.y].tag;
	if (info.map[info.marisa.position.x][info.marisa.position.y].type == GridType::Trigger)
		colorUp |= info.map[info.marisa.position.x][info.marisa.position.y].tag;
	for (int i = 0; i < info.map.size().x; ++i)
	{
		for (int j = 0; j < info.map.size().y; ++j)
		{
			if (i == info.marisa.position.x&&j == info.marisa.position.y)
			{
				cout << (info.marisa.have ? 'M' : 'm');
			}
			else if (i == info.minoriko.position.x&&j == info.minoriko.position.y)
			{
				cout << (info.minoriko.have ? 'A' : 'a');

			}
			else
			{
				char out = gridStr[(int)info.map[i][j].type];
				if (info.map[i][j].type == GridType::Trap && (info.map[i][j].tag | colorUp))
					out = '^';
				if (info.map[i][j].type == GridType::MoveableWall && (info.map[i][j].tag | colorUp))
					out = '#';
				cout << out;
			}
		}
		cout << endl;
	}
}
void SimplexAI::printMovements(vector<Movement>& movements)
{

}
std::ostream& operator<<(std::ostream& os, Movement m)
{
	os << (int)m.type+1;
	if ((int)m.type <= 3)os << " " << (int)m.direction;
	if ((int)m.type == 1)os << " " << (int)m.distance;
	return os;
}
Movement SimplexAI::generateMovement(GameInfo info)
{
	State state;
	state.marisa = info.marisa;
	state.minoriko = info.minoriko;
	state.roundLeft = info.roundLimit - info.round;
	//printInfo(info);
	cout << (info.who ? "����ħ��ɳ" : "�����") << endl;
	printf("���Ž�ƽ���÷�:%.4lf\n", info.score+avgScore[state]-1.0);
	double p[10][10];
	memset(p, 0, sizeof(p));
	vector<Movement>MovementMarisa = getMovementForMarisa(state);
	vector<Movement>MovementMinoriko = getMovementForMinoriko(state);
	int szmarisa = MovementMarisa.size(), szminoriko = MovementMinoriko.size();
	int row = szmarisa, col = szminoriko;
	if (info.who == 0)
		swap(row, col);
	//�����������
	for (int i = 0; i < row; ++i)
		for (int j = 0; j < col; ++j)
		{
			Game game;
			game.newGame(startState);
			game.score = 0;
			game.marisa = state.marisa;
			game.minoriko = state.minoriko;
			game.round = game.roundLimit - state.roundLeft;
			game.setMovementForMarisa(MovementMarisa[info.who?i:j]);
			game.setMovementForMinoriko(MovementMinoriko[info.who?j:i]);
			game.roundFinish();
			State nextState;
			nextState.marisa = game.marisa;
			nextState.minoriko = game.minoriko;
			nextState.roundLeft = game.roundLimit - game.round;
			p[i][j] = avgScore[nextState] + game.score;
			if (!info.who)p[i][j] = 9999.0-p[i][j];
		}
	/*
	for (int i = 0; i < szmarisa; ++i)
	{
	for (int j = 0; j < szminoriko; ++j)
	{
	printf("%lf ", p[i][j]);
	}
	cout << endl;
	}*/
	//������ 
	memset(a, 0, sizeof(a));
	memset(id, 0, sizeof(id));
	for (int j = 1; j <= row; ++j)
		a[0][j] = -1.0, id[j] = j;
	for (int i = 1; i <= col; ++i) {
		for (int j = 1; j <= row; ++j)
			a[i][j] = -p[j - 1][i - 1];
		a[i][0] = -1.0;
	}
	int i, j, l, e; double k, kk;
	while (true)
	{
		l = e = 0; k = -cns::eps;
		for (i = 1; i <= col; ++i)
			if (a[i][0]<k)
			{
				k = a[i][0];
				l = i;
			}
		if (!l) break;
		k = -cns::eps;
		for (j = 1; j <= row; ++j)
			if (a[l][j] < k && (!e || (rand() & 1)))
			{
				k = a[l][j];
				e = j;
			}
		pivot(l, e, row, col);
	}
	while (true) {
		for (j = 1; j <= row; ++j)
			if (a[0][j] > cns::eps)
				break;
		if ((e = j) > row) break;
		k = 1e18; l = 0;
		for (i = 1; i <= col; ++i)
			if (a[i][e] > cns::eps && (kk = (a[i][0] / a[i][e])) < k)
			{
				k = kk;
				l = i;
			}

		pivot(l, e, row, col);
	}
	double res[10 << 1];
	memset(res, 0, sizeof(res));
	for (i = 1; i <= col; ++i)res[id[row + i]] = a[i][0];
	double ep = 0.0;
	for (i = 1; i <= row; ++i)ep += res[i];
	ep = 1.0 / ep;
	Movement result = Movement::createMovementStay();
	bool ok = false;
	double randNum = rand() % 1000;
	randNum /= 1000.0;
	//printf("%lf", randNum);
	if (info.who)
	{
		for (i = 1; i <= row; ++i)
		{
			if ((int)round(res[i] * ep*100.0) > 0)
			{
				//cout<<printMovement(MovementMarisa[i - 1])<<" ����:"<< (int)round(res[i] * ep * 100.0)<<endl;
			}
			if (!ok&&randNum < res[i] * ep)
			{
				result = MovementMarisa[i - 1];
				ok = true;
			}
			randNum -= res[i] * ep;
		}
	}
	else
	{
		for (i = 1; i <= row; ++i)
		{
			if ((int)round(res[i] * ep*100.0) > 0)
			{
				//cout << printMovement(MovementMinoriko[i - 1]) << " ����:" << (int)round(res[i] * ep * 100.0) << endl;
			}
			if (!ok&&randNum < res[i] * ep)
			{
				result = MovementMinoriko[i - 1];
				ok = true;
			}
			randNum -= res[i] * ep;
		}
	}
	cout << endl;
	return result;
}