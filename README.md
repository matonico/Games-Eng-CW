# Games Engineering Coursework
Coursework repo for games eng, platformer game.  
Matthew Lenathen - 40506678  
Medárd Donát Seff - 40498106  

Project flow
---
Master branch reserved for big milestones, develop branch used as main working branch. For every new feature, a new branch was created then merged into develop once complete.  

Cloning
---

For cloning on a new environment remember to use:  
```
git submodule update --init --recursive
``` 
after cloning


Implementing a new feature branch
---
```
git checkout -b branch-name develop
```      
Do work as normal, then:
```
git add .
git commit -m "message" 
git push -u origin branch-name
```
Merging feature branch to develop
---
```
git checkout develop
git pull
git merge branch-name
git push
```
