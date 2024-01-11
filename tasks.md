Sur cette branche (dev), le but est de passer par un dictionnaire d'objets permettant d'avoir une interface avec des entries déportés.
Pour ce faire, j'ai déjà fait la classe LocalObjectEntry pour les entries locaux.
Pour les entries déportés, il faut faire une autre classe (RemoteObjectEntry ?), et implémenter les méthodes IObjectEntry::readBytes(uint32_t readSize, uint32_t offset, uint8_t *buff) et IObjectEntry::writeBytes(uint32_t writeSize, uint32_t offset, uint8_t *buff).
Il faudra aussi modifier le générateur Python pour avoir les nouvelles classes autogénérées, ainsi qu'un code source .cpp qui contient les getters/setters des remote entries.
Puisque la class Object a changé, il faut aussi adapter toutes les sous-classes dabs src/objects, en s'aidant du code sur la branche main.
Il faut aussi réimplémenter les limites, pour les entries qui ont un min-max.
Pour le générateur Python, la nouvelle version n'a plus du tout de vérification des données comme il y avait avant, il faut donc se baser sur la branche main pour éventuellement remettre les vérifications.
Puisque les valeurs remote ne sont pas tout de suite disponibles, il faut faire avec pour les PDOs et SDO. L'utilisation de la librairie C++ future peut être utile pour ça.