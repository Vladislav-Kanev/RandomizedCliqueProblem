mkdir -p data
cd data

URL_TO_THE_FILE="https://www.dcs.gla.ac.uk/~pat/jchoco/clique/dimacs/DIMACS_cliques/"

files=(
    "brock200_1.clq"
    "brock200_2.clq"
    "brock200_3.clq"
    "brock200_4.clq"
    "brock400_1.clq"
    "brock400_2.clq"
    "brock400_3.clq"
    "brock400_4.clq"
    "C125.9.clq"-
    "gen200_p0.9_44.clq"
    "gen200_p0.9_55.clq"
    "hamming8-4.clq"
    "johnson16-2-4.clq"
    "johnson8-2-4.clq"
    "keller4.clq"
    "MANN_a27.clq"
    "MANN_a9.clq"
    "p_hat1000-1.clq"
    "p_hat1000-2.clq"
    "p_hat1500-1.clq"
    "p_hat300-3.clq"
    "p_hat500-3.clq"
    "san1000.clq"
    "sanr200_0.9.clq"
    "sanr400_0.7.clq"
)

for file in "${files[@]}"; do
    wget "$URL_TO_THE_FILE/$file"
done