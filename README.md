cmake --preset debug && cmake --build --preset debug
ctest --preset debug


Workflow

Initialiser le projet : créer option-pricer/src/main.cpp, ajouter la cible add_executable(...) et lier vos libs dans option-pricer/CMakeLists.txt.
Configuration : cmake --preset debug (ou release/asan/ubsan selon le besoin) — cela génère les fichiers de build dans build/<preset>.
Compilation : cmake --build --preset debug utilise Ninja pour produire les binaires/tests.
Tests : ctest --preset debug lance les tests enregistrés ; remplace debug par un autre preset si tu veux tester l’exécutable construit avec d’autres flags.
(Optionnel) Outils : lance clang-format et clang-tidy en t’appuyant sur les configs fournies lorsque tu veux uniformiser le formatage ou vérifier les diagnostics.
Intégration : commits des sources + configs, jamais des répertoires build/; push ou archive ensuite.
Tu peux enchaîner ces étapes à chaque itération de dev pour éviter d’avoir à redemander la procédure.