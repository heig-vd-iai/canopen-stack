# Limitations

## Fonctions absentes

- L'objet TIME n'est pas géré.
- Le mapping PDO est limité à 8 objets, car la granularité vaut 8, soit un mapping à
  l'octet.
- Les COB-ID des PDO et du SYNC sont lus dans le dictionnaire d'objets. Les autres
  services utilisent le jeu de connexions prédéfini, CiA 301:2011 §7.3.3.
- Les MPDO ne sont pas gérés.
- Un seul serveur SDO. Pas de client SDO.
- Le consommateur heartbeat n'est pas géré.
- Les commandes du système d'exploitation ne sont pas gérées.
- Le LSS n'est pas géré.
- Les drapeaux d'objet (`ObjFlags`) ne sont pas gérés.

## Comportements à connaître

- Le compteur SYNC suit la valeur reçue quand le message en porte une. Sinon il
  s'incrémente jusqu'au débordement de l'objet `0x1019`, ou jusqu'à 240 quand l'objet
  est absent.
- Le PST du transfert par blocs SDO est accepté puis ignoré. Le serveur répond toujours
  à une demande de bloc par un transfert par blocs.
- Les objets `VISIBLE_STRING` ont un texte par défaut et une capacité, mais le serveur
  SDO ne les transfère pas encore.
- Un programme ne contient qu'un seul nœud. Les tables du dictionnaire d'objets sont
  statiques et le numéro du nœud vient de `od.hpp`.
- La pile ne protège aucune section critique. Voir
  [vue d'ensemble](../architecture/vue-ensemble.md).

## Délais par défaut

| Réglage | Valeur | Macro |
|---|---|---|
| Transfert SDO | 1 s | `CANOPEN_SDO_TIMEOUT_US` |
| Sous-bloc SDO | 100 ms | `CANOPEN_SDO_BLOCK_TIMEOUT_US` |
| Accès à un objet distant, côté SDO | 100 ms | `CANOPEN_SDO_REMOTE_TIMEOUT_US` |
| Accès à un objet distant, côté application | 10 ms | aucune |

Les trois premiers se règlent à la compilation, voir `src/sdo/config.hpp`. Le dernier
est la constante `REMOTE_ACCESS_TIMEOUT_US` de `src/od_common.cpp`.

## Objets ignorés ou non implémentés

| Index | Nom |
|---|---|
| `0x1006` | Producteur SYNC |
| `0x1012` | COB-ID TIME, la valeur standard est utilisée |
| `0x1013` | Estampille temporelle haute résolution |
| `0x1014` | COB-ID EMCY, la valeur standard est utilisée |
| `0x1015` | Temps d'inhibition EMCY |
| `0x1016` | Consommateur heartbeat |
| `0x1020` | Vérification de la configuration |
| `0x1021` | Stockage de l'EDS |
| `0x1023` | Commandes du système d'exploitation |
| `0x1027` | Appareils modulaires |
| `0x1028` | Consommateur d'urgence |
| `0x1200`–`0x12FF` | Serveurs et clients SDO supplémentaires |
| `0x1FA0`–`0x1FFF` | MPDO |
