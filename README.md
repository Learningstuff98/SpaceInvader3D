This is just a small side project that I'm making in my ongoing journey to learn how to make games with the Unreal Engine and C++


https://github.com/user-attachments/assets/94d52901-9786-4a0e-9ad6-173b9131e83c

Here are some of the features that I'm proud of:

I used the UE5 niagara system for the engine thruster effect. The color is determined by current speed.

https://github.com/user-attachments/assets/3d7b08a5-e7ea-452e-8d69-900b27ec13cc

The ship does have blasters! I used the niagara system for this feature as well.

https://github.com/user-attachments/assets/027c0c59-a10a-42bf-a5a4-98648cf520c7

The Asteroids do spin! The spinning logic was written with C++

https://github.com/user-attachments/assets/6cce2580-89fb-47b4-afbf-ea26f8f2e2e6

4/30/2025

The ship can crash and explode! For this I had to learn how to use field systems to break apart geometry collections. Also I got more experience using the UE5 niagara system.

[Link to PR #22](https://github.com/Learningstuff98/SpaceInvader3D/pull/22)

https://github.com/user-attachments/assets/5e730b32-4030-4108-b0e2-62693571833e

5/10/2025

The ship now has a space themed skybox attached to it. This adds a very nice outer space feel.

[Link to PR #23](https://github.com/Learningstuff98/SpaceInvader3D/pull/23)

https://github.com/user-attachments/assets/12996951-83be-45b0-bbb1-5425fcb59dbb

Adding the outer space theme made things a little dark so I decided to add some headlights that can be toggled on and off.

https://github.com/user-attachments/assets/942d3a5f-26ed-43d3-82d8-c4abf8e6c164


5/22/2025

There are now enemy ships which have the ability to follow the player ship. If no player ship is detected, then enemy ships
fly between hand placed patrol targets.

[Link to to PR #24](https://github.com/Learningstuff98/SpaceInvader3D/pull/24)

https://github.com/user-attachments/assets/3bf13bad-74a0-47f2-989b-89ecca1ffa78


https://github.com/user-attachments/assets/6954ae09-8e46-4562-a2cb-bbf7976f8791

6/3/2025

Enemy ships can be blown up! Hooray!

[Link to PR #25](https://github.com/Learningstuff98/SpaceInvader3D/pull/25)

https://github.com/user-attachments/assets/19d0fadb-f5f2-40f0-806f-f6919e58b229


6/27/2025

Added enemy tracking missiles.

The player ship can lock onto enemy ships by getting within range and pointing roughly at them.
This causes a red outline box to appear to show the player which target they locked onto.

Missiles have to be reloaded before firing again. Reload progress is shown with a purple progress
bar.

[Link to PR #28](https://github.com/Learningstuff98/SpaceInvader3D/pull/28/files)

https://github.com/user-attachments/assets/fea0ab75-4261-4377-b4dc-4ef58ec654f8



