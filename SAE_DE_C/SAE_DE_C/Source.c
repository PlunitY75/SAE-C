#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#pragma warning(disable: 4996 6031)

enum {
	NB_SEMESTRES = 2,
	MIN_UE = 3,
	MAX_UE = 6,
	MAX_MATIERES = 10,
	MAX_EPREUVES = 5,
	MAX_ETUDIANTS = 100,
	MAX_CHAR = 30,
	MAX_NB_NOTES = 100
};


const float MIN_NOTE = 0.f, MAX_NOTE = 20.f;

typedef char CH30[MAX_CHAR + 1];

typedef struct {
	CH30 nomMat;
	CH30 nomEpr;
	unsigned int numSemestre;
	float note;
} Note;

typedef struct {
	CH30 nom;
	unsigned int nbNotes;
	Note notes[MAX_NB_NOTES];
} Etudiant;

typedef struct {
	CH30 nom;
	unsigned int nbCoef;
	float coeff[MAX_UE];
} Epreuve;

typedef struct {
	CH30 nom;
	unsigned int nbEpreuves;
	Epreuve epreuves[MAX_EPREUVES];
} Matiere;

typedef struct {
	unsigned int nbMatieres;
	Matiere matieres[MAX_MATIERES];
} Semestre;

typedef struct {
	unsigned int nbEtudiant;
	unsigned int nbUE; // nombre de coef, commun à toutes les épreuves
	Semestre semestres[NB_SEMESTRES];
	Etudiant etudiants[MAX_ETUDIANTS];
} Formation;

//C1
/*void sortir() {
	exit(0);
}*/


//C2

// 
void formation(Formation* formation) {
	int ue;
	scanf("%d", &ue);
	if (formation->nbUE != 0) {
		printf("Le nombre d'UE a deja ete defini\n");
	}
	else if (ue >= MIN_UE && ue <= MAX_UE) {
		printf("Le nombre d'UE est defini\n");
		formation->nbUE = ue;
	}
	else
		printf("Le nombre d'UE est incorrect\n");
}


//C3

void epreuve(Formation* ff) {
	unsigned int numSem = 0;
	const char nomMatiere[MAX_CHAR + 1];
	const char nomEpreuve[MAX_CHAR + 1];
	int indiceMatiere = -1; //Initialisation de matière a une valeur 
	int indiceEpreuve = -1; //négative = non trouvée/inconnue.
	float coeffsUE[MAX_UE];
	float sommeCoeffs = 0.0;
	//Verification nombre d'UE déjà défini
	if (ff->nbUE == 0) {
		printf("Le nombre d'UE n'est pas defini\n");
		return;
	}


	//Scan des données entrées par l'utilisateur
	scanf("%u", &numSem);
	scanf("%s", nomMatiere);
	scanf("%s", nomEpreuve);

	//Autant de coefficients que de nombre d'UE
	for (unsigned int i = 0; i < ff->nbUE; i++) {
		scanf("%f", &coeffsUE[i]);
	}

	//Verification de la validité des coeffs
	for (unsigned int j = 0; j < ff->nbUE; ++j) {
		sommeCoeffs += coeffsUE[j];
		if (coeffsUE[j] < 0) {
			printf("Au moins un des coefficients est incorrect\n");
			return;
		}
	}
	if (sommeCoeffs == 0.0) {
		printf("Au moins un des coefficients est incorrect\n");
		return;
	}

	//Verification de l'entrée du numéro de semestre
	if (numSem < 1 || numSem > NB_SEMESTRES) {
		printf("Le numero de semestre est incorrect \n");
		return;
	}
	else {
		--numSem;
	}

	Semestre* s = &ff->semestres[numSem];


	//Matiere saisie existe-elle déjà?
	for (unsigned int k = 0; k < s->nbMatieres; ++k) {
		if (strcmp(s->matieres[k].nom, nomMatiere) == 0) {
			indiceMatiere = k;
		}
	}

	//Ajout d'une nouvelle matière
	if (indiceMatiere == -1) {
		strcpy(s->matieres[s->nbMatieres].nom, nomMatiere);
		s->matieres[s->nbMatieres].nbEpreuves = 0;
		indiceMatiere = s->nbMatieres;
		s->nbMatieres += 1;
		printf("Matiere ajoutee a la formation\n");
	}

	Matiere* m = &s->matieres[indiceMatiere];

	//Epreuve saisie existe-elle déjà?
	for (unsigned int i = 0; i < m->nbEpreuves; ++i) {
		if (strcmp(m->epreuves[i].nom, nomEpreuve) == 0) {
			indiceEpreuve = i;
			printf("Une meme epreuve existe deja\n");
		}
	}

	if (indiceEpreuve == -1) {
		strcpy(m->epreuves[m->nbEpreuves].nom, nomEpreuve);
		m->epreuves[m->nbEpreuves].nbCoef = ff->nbUE;

		indiceEpreuve = m->nbEpreuves;

		for (int i = 0; i < ff->nbUE; ++i) {
			m->epreuves[indiceEpreuve].coeff[i] = coeffsUE[i];
		}
		m->nbEpreuves += 1;
		printf("Epreuve ajoutee a la formation\n");
	}
}


//C4
void coefficient(Formation* ff)
{
	unsigned int numSem = 0;

	scanf("%u", &numSem);

	if (!(numSem == 1 || numSem == 2))
	{
		printf("Le numero de semestre est incorrect\n");
		return;
	}
	--numSem;
	int epr = 0;
	for (int i = 0; i < ff->semestres[numSem].nbMatieres; ++i)
	{
		if (ff->semestres[numSem].matieres[i].nbEpreuves != 0)
		{
			epr = 1;
			break;
		}
	}

	if (epr == 0)
	{
		printf("Le semestre ne contient aucune epreuve\n");
		return;
	}

	for (int i = 0; i < ff->nbUE; ++i)
	{
		int estNul = 1;
		for (int j = 0; j < ff->semestres[numSem].nbMatieres; ++j)
		{
			for (int k = 0; k < ff->semestres[numSem].matieres[j].nbEpreuves; ++k)
			{

				if (ff->semestres[numSem].matieres[j].epreuves[k].coeff[i] != 0)
				{
					estNul = 0;
					break;
				}
			}
		}
		if (estNul == 1) {
			printf("Les coefficients d au moins une UE de ce semestre sont tous nuls.\n");
			return;
		}
	}

	printf("Coefficients corrects\n");
}

//C5

void note(Formation* ff)
{
	unsigned int numSem;
	const char nomEtu[MAX_CHAR + 1];
	const char nomMatiere[MAX_CHAR + 1];
	const char nomEpreuve[MAX_CHAR + 1];
	unsigned int nbEtu;
	unsigned int note;

	scanf("%u", &numSem);
	scanf("%s", nomEtu);
	scanf("%s", nomMatiere);
	scanf("%s", nomEpreuve);
	scanf("%u", &note);
	nbEtu = ff->nbEtudiant;
	if (!(numSem == 1 || numSem == 2))
	{
		printf("Le numero de semestre est incorrect\n");
		return;
	}

	--numSem;
	bool EtuExist = false;
	bool MatExist = false;
	bool EprExist = false;

	for (int i = 0; i < ff->semestres[numSem].nbMatieres; ++i)
	{
		if (strcmp(nomMatiere, ff->semestres[numSem].matieres[i].nom) == 0)
		{
			MatExist = true;
			for (int j = 0; j < ff->semestres[numSem].matieres[i].nbEpreuves; ++j)
			{
				if (strcmp(nomEpreuve, ff->semestres[numSem].matieres[i].epreuves[j].nom) == 0)
				{
					EprExist = true;
					break;
				}
			}
			break;
		}
	}

	if (!MatExist)
	{
		printf("Matiere inconnue\n");
		return;
	}
	if (!EprExist)
	{
		printf("Epreuve inconnue\n");
		return;
	}
	if (note < MIN_NOTE || note> MAX_NOTE)
	{
		printf("Note incorrecte\n");
		return;
	}

	for (int i = 0; i < ff->nbEtudiant; ++i)
	{
		if (strcmp(nomEtu, ff->etudiants[i].nom) == 0)
		{
			EtuExist = true;
			for (int j = 0; j < ff->etudiants[i].nbNotes; j++) {
				if (strcmp(ff->etudiants[i].notes[j].nomEpr, nomEpreuve) == 0
					&& strcmp(ff->etudiants[i].notes[j].nomMat, nomMatiere) == 0
					&& ff->etudiants[i].notes[j].numSemestre == numSem) {
					printf("Une note est deja definie pour cet etudiant\n");
					return;
				}
			}
			nbEtu = i;
			break;
		}
	}

	if (!EtuExist)
	{
		strcpy(ff->etudiants[nbEtu].nom, nomEtu);
		ff->etudiants[nbEtu].nbNotes = 0;
		ff->nbEtudiant++;
		printf("Etudiant ajoute a la formation\n");
	}

	strcpy(ff->etudiants[nbEtu].notes[ff->etudiants[nbEtu].nbNotes].nomEpr, nomEpreuve);
	strcpy(ff->etudiants[nbEtu].notes[ff->etudiants[nbEtu].nbNotes].nomMat, nomMatiere);
	ff->etudiants[nbEtu].notes[ff->etudiants[nbEtu].nbNotes].note = note;
	ff->etudiants[nbEtu].notes[ff->etudiants[nbEtu].nbNotes].numSemestre = numSem;
	ff->etudiants[nbEtu].nbNotes++;
	printf("Note ajoutee a l'etudiant\n");
}

//C6

void notes(Formation* ff) {
	unsigned int numSem;
	const char nomEtu[MAX_CHAR + 1];
	unsigned int indexEtudiant = 0;
	bool etudiantExiste = false;
	unsigned int totalEpreuves = 0;
	unsigned int totalNotes = 0;

	if (!(ff->nbUE)) {
		printf("Le nombre d'UE n'est pas defini\n");
		return;
	}

	scanf("%u", &numSem);
	scanf("%s", nomEtu);

	if (numSem < 1 || numSem > NB_SEMESTRES) {
		printf("Le numero de semestre est incorrect \n");
		return;
	}
	--numSem;
	for (int i = 0; i < ff->nbEtudiant; ++i) {
		if (strcmp(ff->etudiants[i].nom, nomEtu) == 0) {
			indexEtudiant = i;
			etudiantExiste = true;
		}
	}
	if (!etudiantExiste) {
		printf("Etudiant inconnu\n");
		return;
	}
	for (int i = 0; i < ff->semestres[numSem].nbMatieres; ++i) {
		totalEpreuves += ff->semestres[numSem].matieres[i].nbEpreuves;
	}
	for (int i = 0; i < ff->etudiants[indexEtudiant].nbNotes; ++i) {
		if (ff->etudiants[indexEtudiant].notes[i].numSemestre == numSem) {
			++totalNotes;
		}
	}
	if (totalEpreuves != totalNotes) {
		printf("Il manque au moins une note pour cet etudiant\n");
	}
	else {
		printf("Notes correctes\n");
	}
}

//C7

void releve(Formation* ff) {
	unsigned int numSem;
	const char nomEtu[MAX_CHAR + 1];
	unsigned int indexEtudiant = 0;
	bool etudiantExiste = false;
	const char nomMatiere[MAX_CHAR + 1];
	const char nomEpreuve[MAX_CHAR + 1];
	unsigned int nbEtu;
	unsigned int totalEpreuves = 0;
	unsigned int totalNotes = 0;

	if (!(ff->nbUE)) {
		printf("Le nombre d'UE n'est pas defini\n");
		return;
	}

	scanf("%d", &numSem);
	scanf("%s", nomEtu);

	if (!(numSem == 1 || numSem == 2)) {
		printf("Le numero de semestre est incorrect\n");
		return;
	}
	--numSem;
	for (int i = 0; i < ff->nbEtudiant; ++i) {
		if (strcmp(ff->etudiants[i].nom, nomEtu) == 0) {
			indexEtudiant = i;
			etudiantExiste = true;
		}
	}
	if (!etudiantExiste) {
		printf("Etudiant inconnu\n");
		return;
	}

	for (int i = 0; i < ff->semestres[numSem].nbMatieres; ++i)
	{
		for (int j = 0; j < ff->semestres[numSem].matieres[i].nbEpreuves; ++j)
		{
			int cpt = 0;
			for (int k = 0; k < ff->nbUE; ++k)
			{
				if (ff->semestres[numSem].matieres[i].epreuves[j].coeff[k] == 0)
				{
					cpt++;
				}
			}
			if (cpt == 3) {
				printf("Les coefficients de ce semestre sont incorrects\n");
				return;
			}
		}
	}
	for (int i = 0; i < ff->semestres[numSem].nbMatieres; ++i) {
		totalEpreuves += ff->semestres[numSem].matieres[i].nbEpreuves;
	}
	for (int i = 0; i < ff->etudiants[indexEtudiant].nbNotes; ++i) {
		if (ff->etudiants[indexEtudiant].notes[i].numSemestre == numSem) {
			++totalNotes;
		}
	}

	if (totalEpreuves != totalNotes) {
		printf("Il manque au moins une note pour cet etudiant\n");
		return;
	}

	// Pour chaque matiere, on stocke la somme des notes des UEs
	float sommeNoteUe[MAX_MATIERES][MAX_UE];
	float sommeCoefUe[MAX_MATIERES][MAX_UE];
	for (int i = 0; i < MAX_MATIERES; i++) {
		for (int j = 0; j < MAX_UE; j++) {
			sommeNoteUe[i][j] = 0.0;
			sommeCoefUe[i][j] = 0.0;
		}
	}

	for (int i = 0; i < ff->semestres[numSem].nbMatieres; i++) {
		for (int j = 0; j < ff->semestres[numSem].matieres[i].nbEpreuves; j++) {
			for (int k = 0; k < ff->etudiants[indexEtudiant].nbNotes; k++) {

				if (ff->etudiants[indexEtudiant].notes[k].numSemestre == numSem
					&& strcmp(ff->etudiants[indexEtudiant].notes[k].nomMat, ff->semestres[numSem].matieres[i].nom) == 0
					&& strcmp(ff->etudiants[indexEtudiant].notes[k].nomEpr, ff->semestres[numSem].matieres[i].epreuves[j].nom) == 0) {

					for (int l = 0; l < ff->semestres[numSem].matieres[i].epreuves[j].nbCoef; l++) {
						sommeNoteUe[i][l] += ff->etudiants[indexEtudiant].notes[k].note * ff->semestres[numSem].matieres[i].epreuves[j].coeff[l];
						sommeCoefUe[i][l] += ff->semestres[numSem].matieres[i].epreuves[j].coeff[l];
					}
				}
			}
		}
	}

	// Pour connaitre l'offset des entetes du tableau
	int nbMaxChar = strlen("Moyennes");
	for (int i = 0; i < ff->semestres[numSem].nbMatieres; i++) {
		if (nbMaxChar < strlen(ff->semestres[numSem].matieres[i].nom)) {
			nbMaxChar = strlen(ff->semestres[numSem].matieres[i].nom);
		}
	}

	// On commence l'affichage du tableau

	// Code pour l'entete
	for (int i = 0; i < nbMaxChar; i++) printf(" ");
	for (int i = 0; i < ff->nbUE; i++) {
		printf("  UE%d", i + 1);
	}

	// On affiche les lignes 1 a 1
	float operation;
	for (int i = 0; i < ff->semestres[numSem].nbMatieres; i++) {
		printf("\n%s", ff->semestres[numSem].matieres[i].nom);
		for (int x = 0; x < nbMaxChar - strlen(ff->semestres[numSem].matieres[i].nom); x++) {
			printf(" ");
		}
		for (int j = 0; j < ff->nbUE; j++) {
			operation = sommeNoteUe[i][j] / sommeCoefUe[i][j];
			if (operation < 10) printf(" ");
			if (isnan(operation)) printf("   ND");
			else printf(" %0.1f", operation);
		}
	}

	// Calcul moyenne
	printf("\n--");
	printf("\nMoyennes");
	for (int x = 0; x < nbMaxChar - strlen("Moyennes"); x++) {
		printf(" ");
	}
	for (int i = 0; i < ff->nbUE; i++) {
		float sommeNote = 0.0;
		float sommeCoef = 0.0;
		for (int j = 0; j < ff->semestres[numSem].nbMatieres; j++) {
			sommeNote += sommeNoteUe[j][i];
			sommeCoef += sommeCoefUe[j][i];
		}
		operation = sommeNote / sommeCoef;
		if (operation < 10) printf(" ");
		if (isnan(operation)) printf("   ND");
		else printf(" %0.1f", operation);
	}
	printf("\n");
}

void decision(Formation* ff) {
	const char nomEtu[MAX_CHAR + 1];
	unsigned int indexEtudiant = 0;
	bool etudiantExiste = false;
	const char nomMatiere[MAX_CHAR + 1];
	const char nomEpreuve[MAX_CHAR + 1];
	unsigned int nbEtu;
	unsigned int totalEpreuves = 0;
	float moyennesAnnuelles[MAX_UE];
	unsigned int nbUEValidees = 0;

	if (!(ff->nbUE)) {
		printf("Le nombre d'UE n'est pas defini\n");
		return;
	}

	scanf("%s", nomEtu);
	for (int i = 0; i < ff->nbEtudiant; ++i) {
		if (strcmp(ff->etudiants[i].nom, nomEtu) == 0) {
			indexEtudiant = i;
			etudiantExiste = true;
		}
	}
	if (!etudiantExiste) {
		printf("Etudiant inconnu\n");
		return;
	}
	for (int s = 0; s < NB_SEMESTRES; ++s) {
		for (int i = 0; i < ff->semestres[s].nbMatieres; ++i)
		{
			for (int j = 0; j < ff->semestres[s].matieres[i].nbEpreuves; ++j)
			{
				int cpt = 0;
				for (int k = 0; k < ff->nbUE; ++k)
				{
					if (ff->semestres[s].matieres[i].epreuves[j].coeff[k] == 0)
					{
						cpt++;
					}
				}
				if (cpt == 3) {
					printf("Les coefficients d’au moins un semestre sont incorrects\n");
					return;
				}
			}
		}
		for (int i = 0; i < ff->semestres[s].nbMatieres; ++i) {
			totalEpreuves += ff->semestres[s].matieres[i].nbEpreuves;
		}
	}
	if (totalEpreuves != ff->etudiants[indexEtudiant].nbNotes) {
		printf("Il manque au moins une note pour cet etudiant\n");
		return;
	}


	int nbMaxChar = strlen("Moyennes annuelles");
	// Code pour l'entete
	for (int i = 0; i < nbMaxChar; i++) printf(" ");
	for (int i = 0; i < ff->nbUE; i++) {
		printf("  UE%d", i + 1);
	}
	printf("\n");

	for (int i = 0; i < MAX_UE; ++i) {
		moyennesAnnuelles[i] = 0;
	}
	for (int s = 0; s < NB_SEMESTRES; ++s) {

		// Pour chaque matiere, on stocke la somme des notes des UEs
		float sommeNoteUe[MAX_MATIERES][MAX_UE];
		float sommeCoefUe[MAX_MATIERES][MAX_UE];
		float operation;
		for (int i = 0; i < MAX_MATIERES; i++) {
			for (int j = 0; j < MAX_UE; j++) {
				sommeNoteUe[i][j] = 0.0;
				sommeCoefUe[i][j] = 0.0;
			}
		}

		for (int i = 0; i < ff->semestres[s].nbMatieres; i++) {
			for (int j = 0; j < ff->semestres[s].matieres[i].nbEpreuves; j++) {
				for (int k = 0; k < ff->etudiants[indexEtudiant].nbNotes; k++) {

					if (ff->etudiants[indexEtudiant].notes[k].numSemestre == s
						&& strcmp(ff->etudiants[indexEtudiant].notes[k].nomMat, ff->semestres[s].matieres[i].nom) == 0
						&& strcmp(ff->etudiants[indexEtudiant].notes[k].nomEpr, ff->semestres[s].matieres[i].epreuves[j].nom) == 0) {

						for (int l = 0; l < ff->semestres[s].matieres[i].epreuves[j].nbCoef; l++) {
							sommeNoteUe[i][l] += ff->etudiants[indexEtudiant].notes[k].note * ff->semestres[s].matieres[i].epreuves[j].coeff[l];
							sommeCoefUe[i][l] += ff->semestres[s].matieres[i].epreuves[j].coeff[l];
						}
					}
				}
			}
		}
		printf("S%d", s + 1);
		for (int i = 0; i < nbMaxChar - strlen("S?"); i++) printf(" ");
		for (int i = 0; i < ff->nbUE; i++) {
			float sommeNote = 0.0;
			float sommeCoef = 0.0;
			for (int j = 0; j < ff->semestres[s].nbMatieres; j++) {
				sommeNote += sommeNoteUe[j][i];
				sommeCoef += sommeCoefUe[j][i];
			}
			operation = sommeNote / sommeCoef;
			if (operation < 10) printf(" ");
			if (isnan(operation)) printf("   ND");
			else printf(" %0.1f", operation);
			moyennesAnnuelles[i] += operation;
		}
		printf("\n");
	}
	for (int i = 0; i < MAX_UE; ++i) {
		moyennesAnnuelles[i] /= NB_SEMESTRES;
	}

	printf("--\n");
	printf("Moyennes annuelles");
	for (int i = 0; i < ff->nbUE; ++i) {
		if (moyennesAnnuelles[i] < 10) printf(" ");
		printf(" %0.1f", moyennesAnnuelles[i]);
	}
	printf("\n");
	printf("Acquisition");

	bool premier = true;
	for (int i = 0; i < nbMaxChar - strlen("Acquisition"); i++) printf(" ");
	for (int i = 0; i < ff->nbUE; ++i) {
		if (moyennesAnnuelles[i] > 10) {
			if (!premier) {
				printf(",");
			}
			printf(" UE%d", i + 1);
			nbUEValidees += 1;
			premier = false;
		}
	}
	printf("\n");
	printf("Devenir");
	for (int i = 0; i < nbMaxChar - strlen("Devenir") + 1; i++) printf(" ");
	if (nbUEValidees > ff->nbUE) {
		printf("Redoublement\n");
	}
	else {
		printf("Passage");
	}
	printf("\n");
}




// Dans le main on fait seulement des appels de fonction
int main() {
	char cde[31] = "";
	Formation ff = {
		.nbUE = 0,
		.semestres[0].nbMatieres = 0,
		.semestres[1].nbMatieres = 0,
		.nbEtudiant = 0
	};

	do {
		scanf("%s", cde);
		if (strcmp(cde, "formation") == 0) { // C2
			formation(&ff);
		}
		else if (strcmp(cde, "epreuve") == 0) { // C3
			epreuve(&ff);
		}
		else if (strcmp(cde, "coefficients") == 0) { // C4
			coefficient(&ff);
		}
		else if (strcmp(cde, "note") == 0) { // C5
			note(&ff);
		}
		else if (strcmp(cde, "notes") == 0) { // C6
			notes(&ff);
		}
		else if (strcmp(cde, "releve") == 0) { // C7
			releve(&ff);
		}
		else if (strcmp(cde, "decision") == 0) { // C8
			decision(&ff);
		}
	} while (strcmp(cde, "exit") != 0);
}