  #Pointless Wars#
  
  ##Repository guidelines:##
  
  This repository follows a gitflow workflow. There are two fixed branches at the origin:
    master:     stores the official release history. The name of all commits in the 
                master branch must contain a version number tag. MUST be stable.
    develop:    serves as an integration branch for features and assets. SHOULD be stable.
  Other types of branches are created and removed by developers throughout development:
    feature:    stores any type of change on code, bug fixing, addition or removal of 
                features, etc. CAN be broken.
    assets:     stores any type of change on assets.
    rc:         a branch for release candidates.
    hotfix:     branch to fix problems encountered in master that have a quick solution.
  Branch name convention (without <>): 
    feature-<feature-name-or-description>
    assets-<asset-type-or-description>
    rc-<version-number>
    hotfix-<issue-or-bug>
  Programmers workflow:
    Start a new trackable feature branch from the tip of the develop branch. After the 
    work is done, create pull request to merge your changes into develop. When the set of features
    required by the next master iteration is complete, create release candidate branch.
    If release candidate is approved, push it to master. If a small problem with a quick
    solution is found in master, create a hotfix branch to create and test the fix. Then
    push fix to master, remembering to update master version.
    Changes on the project tree should not happen without the consent of the lead programmer.
  Artists workflow (windows):
    (0) Setting up your workspace. Only done once.
        I    Download the GitHub windows client. Google it.
        II   Clone the nidoro/PointlessWars repository.
        III  At the top left corner, select the 'develop' branch by clicking on the button
             whose tooltip says 'Switch to a different branch'.
        III  At the top left corner, click on the icon whose tooltip says 'Create
             new branch'. Give it a name following the name convention.
        IV   At the top right corner, click on 'Publish' to store your branch in the cloud.
    (1) Update your branch.
        I    Open the client.
        II   At the top left corner, select the develop branch.
        III  At the top right corner, click on Sync to download the changes made on develop.
        IV   At the top left corner, click on Compare if that option exists, then select your
             branch on the list.
        V    At the top left corner, click on update from develop. If you can't, it means
             your branch is already up to date with the develop branch.
    (2) Make local changes.
      Do your thing normally. Modify, create or delete files, whatever. After you are done
      with your changes, you have to commit your changes on the git software. If you are
      using the GitHub windows client (software):
        I    Open the client.
        II   At the top left corner, select your branch.
        III  At the top center, click on Changes to view the changes you've made since 
             your last commit.
        IV   Fill the summary and description fields with information about your changes.
        V    Click on commit.
        V    At the top right corner, click on Sync to store your changes in the cloud.
      If you are done with your changes and are ready to submit them for review, go to 3.
      If you want to make more changes before submiting them to review, go to 1.
    (3) Submit your changes to review.
        I    Open the client.
        II   At the top left corner, select your branch.
        III  At the top right corner, click on Pull Request if that option exists
             and go to IV. If it doesn't, it means your last pull request is still
             under review. Click on it and update the pull request information.
        IV   Where it says: from <your-branch> into (...), select the 'develop' branch.
        V    Fill the title and description fields with information about your changes.
        VI   Click on Send Pull Request.
      Go to 1. Loop eternaly.
  Everyone:
    master and develop branches should never be deleted.
    If you can't Sync the develop branch through the GitHub desktop app, pull the remote
    develop branch through the command line.