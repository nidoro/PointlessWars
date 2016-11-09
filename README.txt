==========================================
  Pointless Wars
==========================================
  Repository guidelines:
    This repository follows a gitflow workflow. There are two fixed branches:
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
    Programmers instructions: 
      Start a new feature branch from the tip of the develop branch. After the work is 
      done,  push changes into develop. When the set of features required by the next master 
      iteration is complete, create release candidate branch. If release candidate is 
      approved, push it to master. If a small problem with a quick solution is found in
      master, create a hotfix branch to create and test the fix. Then push fix to 
      master, remembering to update master version.
    Artists instructions:
      Start a new assets branch from the tip of the develop branch. After the work is done,
      commit the changes and create a pull request to merge the changes into the develop
      branch. Keep your branch up to date with the develop branch.