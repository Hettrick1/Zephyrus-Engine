# ⚙️ Zephyrus Engine

Zephyrus est un moteur de jeu 3D développé en C++ dans le cadre d'un projet personnel et scolaire. Il repose sur une architecture **Actor-Component** et intègre des outils modernes pour le rendu, la physique et l'édition de scènes.

---

## 🎯 Objectifs

L’objectif de Zephyrus est de servir à la fois de **bac à sable technique** pour expérimenter des concepts avancés (rendu, architecture, systèmes) et de **base de projet** pour des jeux 3D simples à moyen terme.

---
## ⚠️ Attention : Documentation obsolète

> **La documentation actuelle n'est pas à jour.**  
> Certaines fonctionnalités récentes du moteur ne sont pas encore documentées, et l'architecture du code est en cours de refonte.  
> Une mise à jour est prévue prochainement.

📚 [Consulter la documentation (ancienne version)](https://sites.google.com/artfx.fr/zephyrus-engine-wiki/home)

---

## 📌 À propos

- **Type** : Projet personnel & scolaire  
- **Technos** : C++, OpenGL, GLFW3, Bullet, ImGui  
- **Participants** : Solo  
- **Début** : Février 2025 — *En cours de développement*

---

## ✨ Fonctionnalités principales

- 🎥 **OpenGL Renderer** : Utilise le pipeline de rendu OpenGL moderne
- ⌨️ **Input System** : Géré avec GLFW3
- 🧾 **HUD System & Text Renderer** : Rendu de texte et interface utilisateur intégrés
- 🪐 **3D Physics** : Basé sur Bullet Physics 3.2
- 🧩 **Architecture Actor-Component**
- 🛠️ **Simple Editor** : Permet de créer et modifier des scènes
- 💾 **Load / Save** : Sauvegarde des maps, prefabs d’actor et materials en JSON

> ❌ **Pas inclus** : Audio / Physique 2D / Vulkan

---

<img width="1919" height="1033" alt="ZephyrusEditor" src="https://github.com/user-attachments/assets/1b639f33-276c-419a-a6c8-666ca878e279" />
<img width="1919" height="1033" alt="ZephyrusEditor3" src="https://github.com/user-attachments/assets/06b6a73a-8d0d-404b-9c38-734b4aee8dfb" />
<img width="1919" height="1033" alt="ZephyrusEditor2" src="https://github.com/user-attachments/assets/3badf1a9-470d-4285-b930-888400b8cf58" />

---

## 🧪 Projets réalisés avec Zephyrus

- 🎳 **Bowling** : Simulation simple d’un jeu de bowling
- 🔫 **Doom** : Reproduction du gameplay du premier Doom
- 🌍 **Planets** : Génération de petites planètes via des *tessellation* et *geometry shaders*
- 🚀 **The Descent** : Mouvements en 6DOF utilisant les quaternions

---

## 🔄 Roadmap

Voici les évolutions prévues du moteur :

- ⚡ Refonte de l’architecture interne (en cours)
  - Repartir d'une base saine
  - Utilisation de pool allocators si possible
  - intégrer des outils de debug
- 🌟 Modification du renderer (passer à vulkan ou Nvidia NVRHI)
- 🛠️ Développement continu de l’éditeur

---
