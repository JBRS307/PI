#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef char cbool;
#define true 1
#define false 0

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair* relation, int n, pair new_pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
cbool is_reflexive(pair*, int);

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
cbool is_irreflexive(pair* relation, int n);

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
cbool is_symmetric(pair* relation, int n);

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
cbool is_antisymmetric(pair* relation, int n);

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
cbool is_asymmetric(pair *relation, int n);

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
cbool is_transitive(pair *relation, int n);

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
cbool is_partial_order(pair* relation, int n);

// A total order relation is a partial order relation that is connected
cbool is_total_order(pair* relation, int n, int *domain, int n_domain);

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
cbool is_connected(pair *relation, int n, int *domain, int n_domain);

int find_max_elements(pair* relation, int n, int* max_arr);
int find_min_elements(pair* relation, int n, int* min_arr);
int get_domain(pair *relation, int n, int *domain);
cbool in_arr(int *arr, int elem, int n);

// Case 3:

int composition (pair *R, int n_r, pair *S, int n_s, pair *sor);

// Comparator for pair
int cmp_pair(const void *a, const void *b) {
	pair *p1 = (pair*)a;
	pair *p2 = (pair*)b;

	if(p1->first == p2->first)
		return p1->second - p2->second;
	return p1->first - p2->first;
}

// int insert_int (int *tab, int n, int new_element) {
// }

// Add pair to existing relation if not already there
int add_relation (pair *relation, int n, pair new_pair) {
	for(int i = 0; i < n; i++){
		if(new_pair.first == relation[i].first && new_pair.second == relation[i].second)
			return n;
	}
	if(n < MAX_REL_SIZE){
		relation[n] = new_pair;
		return n+1;
	}else return n;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
	int num_pairs;
	pair new_pair;
	int n = 0;

	scanf("%d", &num_pairs);
	for(int i = 0; i < num_pairs; i++){
		scanf("%d%d", &new_pair.first, &new_pair.second);
		
		n = add_relation(relation, n, new_pair);
	}
	return n;
}

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for(int i = 0; i < n; i++){
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	// to_do = 1;
	switch (to_do) {
		case 1:
			qsort(relation, size, sizeof(pair), cmp_pair);
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			qsort(relation, size, sizeof(pair), cmp_pair);
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size, domain, n_domain));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

int get_domain(pair *relation, int n, int *domain){
	int n_domain = 0;
	for(int i = 0; i < n; i++){
		if(!in_arr(domain, relation[i].first, n_domain)){
			domain[n_domain] = relation[i].first;
			n_domain++;
		}
	}
	for(int i = 0; i < n; i++){
		if(!in_arr(domain, relation[i].second, n_domain)){
			domain[n_domain] = relation[i].second;
			n_domain++;
		}
	}
	return n_domain;
}

cbool in_arr(int *arr, int elem, int n){
	for(int i = 0; i < n; i++){
		if(arr[i] == elem) return true;
	}
	return false;
}

cbool is_reflexive(pair *relation, int n){

	int curr;

	for(int i = 0; i < n;){
		curr = relation[i].first;
		for(int j = i; j <= n; j++){
			if(j == n || relation[j].first != curr)
				return false;
			if(relation[j].first == relation[j].second){
				int k = j;
				while(k < n && relation[k].first == curr) k++;
				if(k == n)
					return true;
				else{
					i = k;
					break;
				}
			}
		}
		if(i == n)
			return false;
	}
}

cbool is_irreflexive(pair* relation, int n){
	for(int i = 0; i < n; i++){
		if(relation[i].first == relation[i].second) return false;
	}
	return true;
}

cbool is_symmetric(pair *relation, int n){
	cbool flag = false;
	for(int i = 0; i < n; i++){
		flag = false;
		for(int j = i; j < n; j++){
			if(relation[j].second == relation[i].first && relation[j].first == relation[i].second){
				flag = true;
				break;
			}
		}
		if(!flag) return false;
	}
	return true;
}

cbool is_antisymmetric(pair* relation, int n){
	for(int i = 0; i < n; i++){
		for(int j = i; j < n; j++){
			if(relation[j].second == relation[i].first && relation[j].first == relation[i].second && i != j)
				return false;
		}
	}
	return true;
}

cbool is_asymmetric(pair *relation, int n){
	for(int i = 0; i < n; i++){
		for(int j = i; j < n; j++){
			if(relation[j].second == relation[i].first && relation[j].first == relation[i].second)
				return false;
		}
	}
	return true;
}

cbool is_transitive(pair *relation, int n){
	cbool flag = false;
	for(int i = 0; i < n; i++){
		for(int j = i+1; j < n; j++){
			if(relation[i].second == relation[j].first){
				for(int k = 0; k < n; k++){
					if(relation[k].first == relation[i].first && relation[k].second == relation[j].second){
						flag = true;
						break;
					}
				}
				if(!flag) return false;
			}
		}
	}
	return true;
}

cbool is_connected(pair *relation, int n, int *domain, int n_domain){
	for(int i = 0; i < n_domain; i++){
		for(int j = i+1; j < n_domain; j++){
			for(int k = 0; k < n; k++){
				if(!((domain[i] == relation[k].first && domain[j] == relation[k].second) ||
					  domain[j] == relation[k].first && domain[i] == relation[k].second))
					return false;
			}
		}
	}
	return true;
}

cbool is_partial_order(pair* relation, int n){
	if(is_reflexive(relation, n) && is_antisymmetric(relation, n) && is_transitive(relation, n))
		return true;
	else
		return false;
}

cbool is_total_order(pair* relation, int n, int *domain, int n_domain){
	if(is_partial_order(relation, n) && is_connected(relation, n, domain, n_domain))
		return true;
	else
		return false;
}

int find_max_elements(pair* relation, int n, int* max_arr){
	cbool flag;
	int n_max = 0;
	for(int i = 0; i < n; i++){
		flag = true;
		for(int j = 0; j < n; j++){
			if(relation[i].second == relation[j].first && relation[j].first != relation[j].second){
				flag = false;
				break;
			}
		}
		if(flag && !in_arr(max_arr, relation[i].second, n_max)){
			max_arr[n_max] = relation[i].second;
			n_max++;
		}
	}
	return n_max;
}

int find_min_elements(pair* relation, int n, int* min_arr){
	cbool flag;
	int n_min = 0;
	for(int i = 0; i < n; i++){
		flag = true;
		for(int j = 0; j < n; j++){
			if(relation[i].first == relation[j].second && relation[j].second != relation[j].first){
				flag = false;
				break;
			}
		}
		if(flag && !in_arr(min_arr, relation[i].first, n_min)){
			min_arr[n_min] = relation[i].first;
			n_min++;
		}
	}
	return n_min;
}

int composition (pair *R, int n_r, pair *S, int n_s, pair *sor){
	int n_comp = 0;
	pair new_pair;

	for(int i = 0; i < n_r; i++){
		for(int j = 0; j < n_s; j++){
			if(R[i].second == S[j].first){
				new_pair.first = R[i].first;
				new_pair.second = S[j].second;
				n_comp = add_relation(sor, n_comp, new_pair);
			}
		}
	}
	return n_comp;
}