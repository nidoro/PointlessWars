#    Pointless Wars
Este documento requer revisão.
  
###  Geral:

Este repositório possui uma estrutura gitflow. Existem dois branches públicos:

 * master:     versões que forão/serão destinadas ao público externo. Contém o histórico
               de lançamentos oficiais. Os commits das versões oficiais de lançamento são rotulados
               com o número da versão.
 * develop:    para uso do público interno. Branch para integração de alterações propostas por 
               branches privados.

Os seguintes são tipos branches privados:

 * code:       para qualquer alteração no código.
 * resource:   para alterações em qualquer recurso.
 * external:   para alterações em bibliotecas ou recursos externos.
 * rc:         para candidatos a lançamentos oficiais.
 * hotfix:     para consertos rápidos das versões lançadas oficialmente.
 
O nome de um branch privado DEVE ser prefixado com o seu tipo.
Exemplos: resource/animacao, code/gui_system

Embora qualquer um possa visualizar e modificar branches privados, a intenção é que um branch
privado seja para uso (pasme) privado, significando que antes que o criador do branch solicite
integração em develop (pull request), nenhum outro usuário deste repositório deve fazer alterações
no branch sem o consentimento do criador. Após feito Pull Request, qualquer um pode alterar
o branch e atualizar o Pull Request.

###  Fluxograma para artistas usando GitHub desktop app:

#### Passo 0: Configurando seu espaço de trabalho. Feito apenas uma vez.

 1. Baixe e instale o GitHub desktop app. Google it.
 2. Abra e faça login na sua conta GitHub.
 3. No canto superior direito, clique no +, selecione 'Clone' e clone
    o repositório nidoro/PointlessWars. Se ele não aparece na sua lista de repositórios,
    contate o administrador.
 4. No canto superior esquerdo, clique no ícone cuja tooltip diz 'Create new branch'.
    Dê um nome para o novo branch e selecione 'From branch develop'.
    
#### Passo 1: Atualize seu branch.

 1. Abra o GitHub desktop app.
 2. No canto superior esquerdo, selecione o branch 'develop' clicando no botão
    cuja tooltip diz 'Switch to a different branch'.
 3. No canto superior direito, clique em Sync sincronizar seu repositório local com
    o repositório pai.
 4. No canto superior esquerdo, clique em Compare se essa opção existe, e então
    selecione seu branch na lista.
 5. No canto superior esquerdo, clique em 'Update from develop'. Se essa opção estiver
    desabilitada, significa que seu branch já está atualizado.

#### Passo 2: Faça suas modificações.

Faça seu trabalho normalmente, crie, delete e modifique arquivos como queiser. Após
terminar suas alterações, você precisa salvar (commit) suas alterações no repositório.
Para isso:

 1. Abra o GitHub desktop app.
 2. No canto superior esquerdo, selecione seu branch clicando no botão
    cuja tooltip diz 'Switch to a different branch'.
 3. No topo e no centro, clique em Changes para visualizar e salvar (commit) suas
    alterações.
 4. Preencha os campos summary (obrigatório) e description (opcional) com informações
    a respeito das suas alterações.
 5. Clique em commit.
 6. No canto superior direito, clique em Sync para enviar suas alterações para o
    repositório pai. Se você já terminou suas alterações e está pronto para submetê-las
    para revisão do público interno, vá para o Passo 3. Se você quer fazer mais alterações
    antes de submeter à revisão, continue executando o Passo 2 até que esteja pronto.

#### Passo 3: Submeta suas alterações para revisão.

 1. Abra o GitHub desktop app.
 2. No canto superior esquerdo, selecione seu branch clicando no botão
    cuja tooltip diz 'Switch to a different branch'.
 3. No canto superior esquero, clique em Pull Request se essa opção existe
    e vá para o Passo 3.4. Se essa opção não existe, significa que sua última submissão
    ainda está sob revisão. Clique nela para atualizar as informações com suas
    novas alterações.
 4. Selecione a opção "from 'your-branch' into develop".
 5. Preencha os campos title e description (ambos obrigatórios) com informações a respeito
    das suas alterações. Quanto mais alterações, mais detalhista você deve ser.
 6. Clique em Send Pull Request. Vá para o Passo 1. Repita eternamente.
 
================================================================================================
================================================================================================

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

