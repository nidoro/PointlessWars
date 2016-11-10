#    Pointless Wars
  
###  Repository guidelines:
  
This repository follows a gitflow workflow. There are two public branches:

 * master:     stores the official release history. Release commits are
               tagged with the version number.
 * develop:    serves as an integration branch for private branches.
 
The following are called private branches:

 * code:       stores any type of change on code.
 * resource:   stores any type of change on resources.
 * external:   stores any type of change on external libraries or resources.
 * rc:         a branch for release candidates.
 * hotfix:     branch to fix issues encountered in master that have a quick solution.

Although anyone can see and modify these branches, it is intended that a private
branch is for private use only, meaning that before the branch owner request a pull
from the develop branch, no other team member should make commits on that branch
without the consent of the owner.

A branch name MUST be prefixed with the branch type followed by a slash.
Examples: resource/animations, code/gui_system

###  Programmers workflow:

Start a new trackable code branch from the tip of the develop branch. After the 
work is done, create pull request to merge your changes into develop. When the set of features
required by the next master iteration is complete, create a release candidate branch.
If the PR is approved, merge into master. If a small problem with a quick
solution is found in master, create a hotfix branch to create and test the fix. Then
create a PRto merge it into master, remembering to update master version.

###  Artists workflow (using GitHub desktop app):

#### Step 0: Set up your workspace. Only done once.

 1. Download the GitHub windows client. Google it.
 2. Clone the nidoro/PointlessWars repository.
 3. At the top left corner, select the 'develop' branch by clicking on the button
    whose tooltip says 'Switch to a different branch'.
 4. At the top left corner, click on the icon whose tooltip says 'Create
    new branch'. Give it a name following the name convention.
 5. At the top right corner, click on 'Publish' to store your branch in the cloud.
 
#### Step 1: Update your branch.

 1. Open the client.
 2. At the top left corner, select the develop branch.
 3. At the top right corner, click on Sync to download the changes made on develop.
 4. At the top left corner, click on Compare if that option exists, then select your
    branch on the list.
 5. At the top left corner, click on update from develop. If you can't, it means
    your branch is already up to date with the develop branch.

#### Step 2: Make local changes.

Do your thing normally. Modify, create or delete files, whatever. After you are done
with your changes, you have to commit your changes on the git software. If you are
using the GitHub windows client (software):

 1. Open the client.
 2. At the top left corner, select your branch.
 3. At the top center, click on Changes to view the changes you've made since 
    your last commit.
 4. Fill the summary and description fields with information about your changes.
 5. Click on commit.

At the top right corner, click on Sync to store your changes in the cloud.
If you are done with your changes and are ready to submit them for review, go to 3.
If you want to make more changes before submiting them to review, go to 1.

#### Step 3: Submit your changes to review.

 1. Open the client.
 2. At the top left corner, select your branch.
 3. At the top right corner, click on Pull Request if that option exists
    and go to IV. If it doesn't, it means your last pull request is still
    under review. Click on it and update the pull request information.
 4. Where it says: from <your-branch> into (...), select the 'develop' branch.
 5. Fill the title and description fields with information about your changes.
 6. Click on Send Pull Request.

Go to Step 1. Loop eternaly.

