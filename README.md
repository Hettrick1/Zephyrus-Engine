# ⚙️ Zephyrus Engine

Zephyrus est un moteur de jeu 3D développé en C++ dans le cadre d'un projet personnel et scolaire. Il repose sur une architecture **Actor-Component** et intègre des outils modernes pour le rendu, la physique et l'édition de scènes.

---

## ⚠️ Attention : Documentation obsolète

> **La documentation actuelle n'est pas à jour.**  
> Certaines fonctionnalités récentes du moteur ne sont pas encore documentées, et l'architecture du code est en cours de refonte.  
> Une mise à jour est prévue prochainement.

📚 [Consulter la documentation (ancienne version)](https://sites.google.com/artfx.fr/zephyrus-engine-wiki/home)

---

## 📌 À propos

- **Type** : Projet personnel & scolaire  
- **Technos** : C++, OpenGL, SDL2, Bullet  
- **Participants** : Solo  
- **Début** : Février 2025 — *En cours de développement*

---

## ✨ Fonctionnalités principales

- 🎥 **OpenGL Renderer** : Utilise le pipeline de rendu OpenGL moderne
- ⌨️ **Input System** : Géré avec SDL2
- 🧾 **HUD System & Text Renderer** : Rendu de texte et interface utilisateur intégrés
- 🪐 **3D Physics** : Basé sur Bullet Physics 3.2
- 🧩 **Architecture Actor-Component**
- 🛠️ **Simple Editor** : Permet de créer et modifier des scènes
- 💾 **Load / Save** : Sauvegarde des maps et prefabs d’actor en JSON

> ❌ **Pas inclus** : Audio / Physique 2D / Materials / Vulkan

---

## 🧪 Projets réalisés avec Zephyrus

- 🎳 **Bowling** : Simulation simple d’un jeu de bowling
- 🔫 **Doom** : Reproduction du gameplay du premier Doom
- 🌍 **Planets** : Génération de petites planètes via des *tessellation* et *geometry shaders*

---

## 🔄 Roadmap

Voici les évolutions prévues du moteur :

- ⚡ Refonte de l’architecture interne (en cours)
  - Remplacement des singletons
  - Utilisation de `smart_ptr`
  - Amélioration de l'abstraction
- 🌟 Ajout de systèmes de **lumières**
- 🛠️ Développement continu de l’éditeur

---

## 🎯 Objectifs

L’objectif de Zephyrus est de servir à la fois de **bac à sable technique** pour expérimenter des concepts avancés (rendu, architecture, systèmes) et de **base de projet** pour des jeux 3D simples à moyen terme.

---
