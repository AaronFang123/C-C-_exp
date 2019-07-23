#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

#define INF 32767
#define MAXV 10

typedef struct MS{
	int start;
	int end;
	int length;
	int aph[MAXV];

}elm[MAXV];

typedef struct MS1 {
	elm n[MAXV][MAXV];
	int i;
}SP;

//���õ�ע��
struct edge {
	int u;//�ߵ����
	int v;//�ߵ��յ�
	int w;//Ȩֵ
}edges[MAXV];

typedef struct {
	int no;			//������
	char info[MAXV];		//����������Ϣ
}VType;//��������

typedef struct {
	int edges[MAXV][MAXV]; //�ڽӾ���
	int n;				   //������
	int e;				   //����
	VType vexs[MAXV];	   //��Ŷ�����Ϣ
}MatGraph;//ͼ�ڽӾ�������

void Dispath(MatGraph g, int dist[], int path[], int S[], int v,int m);
//����ڽӾ���g
void DispMat(MatGraph g)	
{
	int i, j;
	for (i = 0;i < g.n;i++)
	{
		for (j = 0;j < g.n;j++)
			if (g.edges[i][j] != INF)
				printf("%4d", g.edges[i][j]);
			else
				printf("%4s", "��");
		printf("\n");
	}
}
void Dijkstra(MatGraph g, int v,int m)	//Dijkstra�㷨
{
	int dist[MAXV], path[MAXV];
	int S[MAXV];				//S[i]=1��ʾ����i��S��, S[i]=0��ʾ����i��U��
	int Mindis, i, j, u=0;
	for (i = 0;i < g.n;i++)
	{
		dist[i] = g.edges[v][i];	//�����ʼ��
		S[i] = 0;					//S[]�ÿ�
		if (g.edges[v][i] < INF)	//·����ʼ��
			path[i] = v;			//����v������i�б�ʱ���ö���i��ǰһ������Ϊv
		else
			path[i] = -1;			//����v������iû��ʱ���ö���i��ǰһ������Ϊ-1
	}
	S[v] = 1;path[v] = 0;			//Դ����v����S��
	for (i = 0;i < g.n - 1;i++)			//ѭ��ֱ�����ж�������·�������
	{
		Mindis = INF;				//Mindis����󳤶ȳ�ֵ
		for (j = 0;j < g.n;j++)		//ѡȡ����S�У���U�У��Ҿ�����С���·�����ȵĶ���u
			if (S[j] == 0 && dist[j] < Mindis)
			{
				u = j;
				Mindis = dist[j];
			}
		S[u] = 1;					//����u����S��
		for (j = 0;j < g.n;j++)		//�޸Ĳ���S�У���U�У��Ķ�������·��
			if (S[j] == 0)
				if (g.edges[u][j] < INF && dist[u] + g.edges[u][j] < dist[j])
				{
					dist[j] = dist[u] + g.edges[u][j];
					path[j] = u;
				}
	}
	Dispath(g, dist, path, S, v, m);	//������·��
}

void Dispath_floyd(MatGraph g, int A[][MAXV], int path[][MAXV])	//������·��
{
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, "AllPath.dat", "w");
	SP sp;
	int i, j, k, s,f=0;
	int apath[MAXV], d;		//���һ�����·���м䶥��(����)���䶥�����
	for (i = 0;i < g.n;i++)
		for (j = 0;j <g.n;j++) {
			if (A[i][j] != INF && i != j)
			{
				fprintf(fp, "%s\t%s\t", g.vexs[i].info, g.vexs[j].info);
				k = path[i][j];
				d = 0; apath[d] = j;
				while (k != -1 && k != i) {
					d++; apath[d] = k;
					k = path[i][k];
				}
				d++; apath[d] = i;
				fprintf(fp, "%s\t", g.vexs[apath[d]].info);
				for (s = d - 1;s >= 0;s--)
					fprintf(fp, "%s\t", g.vexs[apath[s]].info);

				fprintf(fp, "%d\n", A[i][j]);

			}
		}
	fclose(fp);
	printf("����·����Ϣ�Ѿ�������AllPath.dat��\n");
	FILE *fp1;
	errno_t err1;
	err1 = fopen_s(&fp1, "AllPath1.dat", "w");
	for (i = 0;i < g.n;i++)
		for (j = 0;j < g.n;j++)
		{
			if (A[i][j] != INF && i != j)			//������i��j֮�����·��
			{
				printf("  ��%d��%d��·��Ϊ:", i, j);

				k = path[i][j];
				d = 0; apath[d] = j;				//·��������յ�
				while (k != -1 && k != i)			//·��������м��
				{
					d++; apath[d] = k;
					k = path[i][k];
				}
				d++; apath[d] = i;				
				printf("%d", apath[d]);			
				int s1 = d;
				for (s = d - 1;s >= 0;s--) {		
					printf(",%d", apath[s]);
				}
				printf("     \t·������Ϊ:%d\n", A[i][j]);
				sp.n[i][j]->start = i;
				sp.n[i][j]->end = j;
				for (f = 0;f < MAXV;f++) {
					sp.n[i][j]->aph[f] = apath[f];
				}
				sp.n[i][j]->length = A[i][j];
				sp.i = s1;
			}
		}

	fwrite(&sp,sizeof(sp),1,fp);
	fclose(fp1);
	
}
void Dispath(MatGraph g, int dist[], int path[], int S[], int v, int i)
//����Ӷ���v�������������·��
{
	int j, k;
	int apath[MAXV], d;					//���һ�����·��(����)���䶥�����


	if (S[i] == 1 && i != v)
	{
		printf("  �Ӷ���%d%s������%d%s��·������Ϊ:%d\t·��Ϊ:", v, g.vexs[v].info, i, g.vexs[i].info, dist[i]);
		d = 0; apath[d] = i;			//���·���ϵ��յ�
		k = path[i];
		if (k == -1)				//û��·�������
			printf("��·��\n");
		else						//����·��ʱ�����·��
		{
			while (k != v)
			{
				d++; apath[d] = k;
				k = path[k];
			}
			d++; apath[d] = v;		//���·���ϵ����
			printf("%d", apath[d]);	//��������
			for (j = d - 1;j >= 0;j--)	//�������������
				printf(",%d", apath[j]);
			printf("\n");
		}
		
	}
	else {
		printf("����������֮�䲻����·��\n");
	}
}

	void Floyd(MatGraph g)	{					//Floyd�㷨
		int A[MAXV][MAXV], path[MAXV][MAXV];
		int i, j, k;
		for (i = 0;i < g.n;i++)
			for (j = 0;j < g.n;j++)
			{
				A[i][j] = g.edges[i][j];
				if (i != j && g.edges[i][j] < INF)
					path[i][j] = i;
				else
					path[i][j] = -1;
			}
		for (k = 0;k < g.n;k++)
		{
			for (i = 0;i < g.n;i++)
				for (j = 0;j < g.n;j++)
					if (A[i][j] > A[i][k] + A[k][j])
					{
						A[i][j] = A[i][k] + A[k][j];
						path[i][j] = path[k][j];
					}
		}
		Dispath_floyd(g, A, path);
	}

void Create(MatGraph &G) {
	int i,j,k,w=0,p;
	char r[MAXV];
	printf_s("���뾰����n��������������ֱ��������·��������e���ÿո�������");
	scanf_s("%d %d", &G.n, &G.e);
	printf_s("���뾰��ĵ�����Ϣ��\n");
	gets_s(r);
	for (i = 0;i < G.n;i++) {
		printf("�����%d���������ƣ���%d����",i + 1, G.n);
		gets_s(G.vexs[i].info);	
	}  

	for (i = 0;i < G.n;i++) {
		for (j = 0;j < G.n;j++) {
			G.edges[i][j] = INF;
		}
	}

	for (k = 0;k < G.e;k++) {
		printf_s("����·��(vi,vj)�ϵĿ�ʼ������i������������j�����ǵľ���w���ÿո�����:\n");
		printf("�����%d��·������%d����", k + 1, G.e);
		scanf_s("%d %d %d", &i, &j, &w);
		G.edges[i][j] = w;
	}
	for (p = 0;p < G.n;p++) {
		G.edges[p][p] = 0;
	}
}

int put_in_all(MatGraph &g) {
	FILE *fp;
	int r, c;
	errno_t err;
	err =fopen_s(&fp, "input_path.dat", "w");
	if (fp != NULL) {
		for (r = 0;r < g.n;r++) {
			for (c = 0;c < g.n;c++) {
				if (g.edges[r][c] != INF) {
					fprintf_s(fp, "%d\t%s\t%d\t%s\t%d\n", r, g.vexs[r].info, c, g.vexs[c].info, g.edges[r][c]);
				}
			}
		}
		printf_s("���о���;�����Ϣ�ѱ�����input_path.dat�ļ���\n");
		fclose(fp);
		return 1;
	}
	else {
		printf_s("���ļ�ʧ��\n");
		return -1;
	}
		

}

int main(void) {
	int  c=0,k=0,i=0;
	char name1[MAXV],name_b[MAXV];
	char name_e[MAXV];
	MatGraph g;
	FILE *fp2;
	SP sp1;
	errno_t err;
	err = fopen_s(&fp2, "AllPath1.dat", "r");
	if (!fp2) {
		Create(g);
		put_in_all(g);
		printf_s("�ڽӾ���\n");
		DispMat(g);
		printf_s("����Ҫ�������·������㾰�����ƣ�");
		gets_s(name1);
		gets_s(name_b);
		while (c < g.n)
		{
			if (!strcmp(name_b, g.vexs[c].info)) break;
			else c++;
		}
		printf_s("����Ҫ�������·�����յ㾰�����ƣ�");

		gets_s(name_e);
		while (i < g.n)
		{
			if (!strcmp(name_e, g.vexs[i].info)) break;
			else i++;
		}
		printf("%d %d", c, i);
		//	printf("Dijkstra�㷨�����:\n");
		Dijkstra(g, c, i);
		printf("Floyd�㷨�����:\n");
		Floyd(g);

		

		getchar();
		getchar();
		return 0;
	}
	else {
		for (;;) {
			int st, ed,s;
			int count = 0;
			printf("�ļ�AllPath.dat�Ѿ����ڣ�������������յ�����ѯ���·����\n");
			fread(&sp1, sizeof(sp1), 1, fp2);
			scanf_s("%d %d", &st, &ed);
			if (sp1.n[st][ed]->end == ed && sp1.n[st][ed]->start == st) {
				printf("�������·��������Ϊ%d\n", sp1.n[st][ed]->length);
//				for (s = sp1.i - 1;s >= 0;s--) {
//					printf(",%d",sp1.n[st][ed]->aph[s]);
//				}
			}
			else
			{
				printf("���������������·��\n");
			}
		}
		
		fclose(fp2);
		getchar();
		getchar();
	}
	

	
}