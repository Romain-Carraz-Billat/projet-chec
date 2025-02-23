#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

// constantes
const int WIDTH = 800;
const int HEIGHT = 800;
const int BLOCK_SIZE = 100;

sf::Texture textureTourBlanc, textureTourNoir, texturePionBlanc, texturePionNoir, textureRoiBlanc, textureRoiNoir, textureReineBlanche, textureReineNoire, textureFouBlanc, textureFouNoir, textureCavalierBlanc, textureCavalierNoir;

std::vector<sf::Vector2i> calculerCasesAccessibles(sf::Sprite* piece, const sf::Vector2f& position, const std::vector<sf::Sprite>& PiecesBlanches, const std::vector<sf::Sprite>& PiecesNoires) {
    std::vector<sf::Vector2i> accessibleSquares;
    if (!piece) return accessibleSquares;

    bool estNoir = (piece->getTexture() == &textureRoiNoir || piece->getTexture() == &textureReineNoire || piece->getTexture() == &textureTourNoir || piece->getTexture() == &textureFouNoir || piece->getTexture() == &textureCavalierNoir || piece->getTexture() == &texturePionNoir);
    bool estBlanc = !estNoir;

    int x = position.x / BLOCK_SIZE;
    int y = position.y / BLOCK_SIZE;

    if (piece->getTexture() == &texturePionBlanc || piece->getTexture() == &texturePionNoir) {
        int direction = estBlanc ? 1 : -1;
        int startRow = estBlanc ? 1 : 6;
        const std::vector<sf::Sprite>& adversaryPieces = estBlanc ? PiecesNoires : PiecesBlanches;
        const std::vector<sf::Sprite>& ownPieces = estBlanc ? PiecesBlanches : PiecesNoires;

        // Vérification des cases accessibles pour le pion
        bool frontEmpty = std::none_of(PiecesBlanches.begin(), PiecesBlanches.end(), [&](const sf::Sprite& p) {
            return p.getPosition() == sf::Vector2f(x * BLOCK_SIZE, (y + direction) * BLOCK_SIZE);
        }) && std::none_of(PiecesNoires.begin(), PiecesNoires.end(), [&](const sf::Sprite& p) {
            return p.getPosition() == sf::Vector2f(x * BLOCK_SIZE, (y + direction) * BLOCK_SIZE);
        });

        if (y + direction >= 0 && y + direction < 8 && frontEmpty) {
            accessibleSquares.push_back(sf::Vector2i(x, y + direction));
        }

        bool twoSquaresEmpty = frontEmpty && std::none_of(PiecesBlanches.begin(), PiecesBlanches.end(), [&](const sf::Sprite& p) {
            return p.getPosition() == sf::Vector2f(x * BLOCK_SIZE, (y + 2 * direction) * BLOCK_SIZE);
        }) && std::none_of(PiecesNoires.begin(), PiecesNoires.end(), [&](const sf::Sprite& p) {
            return p.getPosition() == sf::Vector2f(x * BLOCK_SIZE, (y + 2 * direction) * BLOCK_SIZE);
        });

        if (y == startRow && y + 2 * direction >= 0 && y + 2 * direction < 8 && twoSquaresEmpty) {
            accessibleSquares.push_back(sf::Vector2i(x, y + 2 * direction));
        }

        if (y + direction >= 0 && y + direction < 8 && x + 1 < 8) {
            bool captureRight = std::any_of(adversaryPieces.begin(), adversaryPieces.end(), [&](const sf::Sprite& p) {
                return p.getPosition() == sf::Vector2f((x + 1) * BLOCK_SIZE, (y + direction) * BLOCK_SIZE);
            });
            if (captureRight) {
                accessibleSquares.push_back(sf::Vector2i(x + 1, y + direction));
            }
        }

        if (y + direction >= 0 && y + direction < 8 && x - 1 >= 0) {
            bool captureLeft = std::any_of(adversaryPieces.begin(), adversaryPieces.end(), [&](const sf::Sprite& p) {
                return p.getPosition() == sf::Vector2f((x - 1) * BLOCK_SIZE, (y + direction) * BLOCK_SIZE);
            });
            if (captureLeft) {
                accessibleSquares.push_back(sf::Vector2i(x - 1, y + direction));
            }
        }
    }

    if (piece->getTexture() == &textureRoiBlanc || piece->getTexture() == &textureRoiNoir) {
        std::vector<sf::Vector2i> kingMoves = {
            {x + 1, y + 1}, {x - 1, y - 1},
            {x - 1, y + 1}, {x + 1, y - 1},
            {x, y + 1}, {x, y - 1},
            {x + 1, y}, {x - 1, y}
        };

        for (const auto& move : kingMoves) {
            int newX = move.x;
            int newY = move.y;

            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                bool occupiedByOwnPiece = std::any_of(estBlanc ? PiecesBlanches.begin() : PiecesNoires.begin(), estBlanc ? PiecesBlanches.end() : PiecesNoires.end(), [&](const sf::Sprite& p) {
                    return p.getPosition() == sf::Vector2f(newX * BLOCK_SIZE, newY * BLOCK_SIZE);
                });

                if (!occupiedByOwnPiece) {
                    accessibleSquares.push_back(sf::Vector2i(newX, newY));
                }
            }
        }
    }

    if (piece->getTexture() == &textureFouBlanc || piece->getTexture() == &textureFouNoir) {
        const std::vector<sf::Sprite>& ownPieces = estBlanc ? PiecesBlanches : PiecesNoires;
        const std::vector<sf::Sprite>& adversaryPieces = estBlanc ? PiecesNoires : PiecesBlanches;

        std::vector<sf::Vector2i> directions = {
            {1, 1}, {-1, -1}, {-1, 1}, {1, -1}
        };

        for (const auto& direction : directions) {
            int newX = x;
            int newY = y;

            while (true) {
                newX += direction.x;
                newY += direction.y;

                if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8) {
                    break;
                }

                bool occupiedByOwnPiece = std::any_of(ownPieces.begin(), ownPieces.end(), [&](const sf::Sprite& p) {
                    return p.getPosition() == sf::Vector2f(newX * BLOCK_SIZE, newY * BLOCK_SIZE);
                });

                if (occupiedByOwnPiece) {
                    break;
                }

                accessibleSquares.push_back(sf::Vector2i(newX, newY));

                bool occupiedByAdversaryPiece = std::any_of(adversaryPieces.begin(), adversaryPieces.end(), [&](const sf::Sprite& p) {
                    return p.getPosition() == sf::Vector2f(newX * BLOCK_SIZE, newY * BLOCK_SIZE);
                });

                if (occupiedByAdversaryPiece) {
                    break;
                }
            }
        }
    }

    if (piece->getTexture() == &textureTourBlanc || piece->getTexture() == &textureTourNoir) {
        const std::vector<sf::Sprite>& ownPieces = estBlanc ? PiecesBlanches : PiecesNoires;
        const std::vector<sf::Sprite>& adversaryPieces = estBlanc ? PiecesNoires : PiecesBlanches;

        std::vector<sf::Vector2i> directions = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

        for (const auto& direction : directions) {
            int newX = x;
            int newY = y;

            while (true) {
                newX += direction.x;
                newY += direction.y;

                if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8) {
                    break;
                }

                bool occupiedByOwnPiece = std::any_of(ownPieces.begin(), ownPieces.end(), [&](const sf::Sprite& p) {
                    return p.getPosition() == sf::Vector2f(newX * BLOCK_SIZE, newY * BLOCK_SIZE);
                });

                if (occupiedByOwnPiece) {
                    break;
                }

                accessibleSquares.push_back(sf::Vector2i(newX, newY));

                bool occupiedByAdversaryPiece = std::any_of(adversaryPieces.begin(), adversaryPieces.end(), [&](const sf::Sprite& p) {
                    return p.getPosition() == sf::Vector2f(newX * BLOCK_SIZE, newY * BLOCK_SIZE);
                });

                if (occupiedByAdversaryPiece) {
                    break;
                }
            }
        }
    }

    if (piece->getTexture() == &textureReineBlanche || piece->getTexture() == &textureReineNoire) {
        const std::vector<sf::Sprite>& ownPieces = estBlanc ? PiecesBlanches : PiecesNoires;
        const std::vector<sf::Sprite>& adversaryPieces = estBlanc ? PiecesNoires : PiecesBlanches;

        std::vector<sf::Vector2i> directions = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Rook-like moves
            {1, 1}, {-1, -1}, {-1, 1}, {1, -1} // Bishop-like moves
        };

        for (const auto& direction : directions) {
            int newX = x;
            int newY = y;

            while (true) {
                newX += direction.x;
                newY += direction.y;

                if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8) {
                    break;
                }

                bool occupiedByOwnPiece = std::any_of(ownPieces.begin(), ownPieces.end(), [&](const sf::Sprite& p) {
                    return p.getPosition() == sf::Vector2f(newX * BLOCK_SIZE, newY * BLOCK_SIZE);
                });

                if (occupiedByOwnPiece) {
                    break;
                }

                accessibleSquares.push_back(sf::Vector2i(newX, newY));

                bool occupiedByAdversaryPiece = std::any_of(adversaryPieces.begin(), adversaryPieces.end(), [&](const sf::Sprite& p) {
                    return p.getPosition() == sf::Vector2f(newX * BLOCK_SIZE, newY * BLOCK_SIZE);
                });

                if (occupiedByAdversaryPiece) {
                    break;
                }
            }
        }
    }

    if (piece->getTexture() == &textureCavalierBlanc || piece->getTexture() == &textureCavalierNoir) {
        const std::vector<sf::Sprite>& ownPieces = estBlanc ? PiecesBlanches : PiecesNoires;

        std::vector<sf::Vector2i> knightMoves = {
            {x + 1, y + 2}, {x + 1, y - 2}, {x - 1, y + 2}, {x - 1, y - 2},
            {x + 2, y + 1}, {x + 2, y - 1}, {x - 2, y + 1}, {x - 2, y - 1}
        };

        for (const auto& move : knightMoves) {
            int newX = move.x;
            int newY = move.y;

            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                bool occupiedByOwnPiece = std::any_of(ownPieces.begin(), ownPieces.end(), [&](const sf::Sprite& p) {
                    return p.getPosition() == sf::Vector2f(newX * BLOCK_SIZE, newY * BLOCK_SIZE);
                });

                if (!occupiedByOwnPiece) {
                    accessibleSquares.push_back(sf::Vector2i(newX, newY));
                }
            }
        }
    }

    return accessibleSquares;
}

bool estechecblanc(const std::vector<sf::Sprite>& PiecesBlanches, const std::vector<sf::Sprite>& PiecesNoires) {
    // Trouver la position du roi
    sf::Vector2i roiBlancPosition;
    for (const auto& piece : PiecesBlanches) {
        if (piece.getTexture() == &textureRoiBlanc) {
            roiBlancPosition = sf::Vector2i(piece.getPosition().x / BLOCK_SIZE, piece.getPosition().y / BLOCK_SIZE);
            break;
        }
    }
    // Vérifier si le roi blanc est menacé
    for (const auto& piece : PiecesNoires) {
        const std::vector<sf::Vector2i> accessibleSquares = calculerCasesAccessibles(const_cast<sf::Sprite*>(&piece), piece.getPosition(), PiecesBlanches, PiecesNoires);
        if (std::find(accessibleSquares.begin(), accessibleSquares.end(), roiBlancPosition) != accessibleSquares.end()) {
            return true;
        }
    }

    return false;
}

bool estechecnoir(const std::vector<sf::Sprite>& PiecesBlanches, const std::vector<sf::Sprite>& PiecesNoires) {
    // Trouver la position du roi
    sf::Vector2i roiNoirPosition;
    for (const auto& piece : PiecesNoires) {
        if (piece.getTexture() == &textureRoiNoir) {
            roiNoirPosition = sf::Vector2i(piece.getPosition().x / BLOCK_SIZE, piece.getPosition().y / BLOCK_SIZE);
            break;
        }
    }
    // Vérifier si le roi noir est menacé
    for (const auto& piece : PiecesBlanches) {
        const std::vector<sf::Vector2i> accessibleSquares = calculerCasesAccessibles(const_cast<sf::Sprite*>(&piece), piece.getPosition(), PiecesBlanches, PiecesNoires);
        if (std::find(accessibleSquares.begin(), accessibleSquares.end(), roiNoirPosition) != accessibleSquares.end()) {
            return true;
        }
    }

    return false;
}


bool validerDeplacement(const sf::Vector2i& newPosition, const std::vector<sf::Vector2i>& accessibleSquares) {
    return std::find(accessibleSquares.begin(), accessibleSquares.end(), newPosition) != accessibleSquares.end();
}

// Ajoutez une variable pour suivre le joueur actuel
bool joueurBlanc = true; // true pour les blancs, false pour les noirs

// Ajoutez une fonction pour vérifier si un roi est capturé
bool estRoiCapture(const std::vector<sf::Sprite>& PiecesBlanches, const std::vector<sf::Sprite>& PiecesNoires) {
    bool roiBlancCapture = std::none_of(PiecesBlanches.begin(), PiecesBlanches.end(), [&](const sf::Sprite& piece) {
        return piece.getTexture() == &textureRoiBlanc;
    });

    bool roiNoirCapture = std::none_of(PiecesNoires.begin(), PiecesNoires.end(), [&](const sf::Sprite& piece) {
        return piece.getTexture() == &textureRoiNoir;
    });

    return roiBlancCapture || roiNoirCapture;
}

void échiquier() {
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Projet Echec");

    // Création des objets représentant les cases de l'échiquier
    sf::RectangleShape caseNoir(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    caseNoir.setFillColor(sf::Color{150, 111, 92});

    sf::RectangleShape caseBlanc(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    caseBlanc.setFillColor(sf::Color{234, 211, 188});

    sf::RectangleShape caseBleue(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    caseBleue.setFillColor(sf::Color{95, 139, 225 }); // Couleur bleue semi-transparente

    sf::RectangleShape caseRouge(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    caseRouge.setFillColor(sf::Color{225, 100, 77 }); // Couleur rouge semi-transparente

    sf::RectangleShape caseVerte(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    caseVerte.setFillColor(sf::Color{0, 255, 0, 128}); // Couleur verte semi-transparente

    // Chargement des textures des pièces
    if (!textureTourBlanc.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces tour blanche.png") ||
        !textureTourNoir.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces tour noire.png") ||
        !texturePionBlanc.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces pion blanc.png") ||
        !texturePionNoir.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces pion noir.png") ||
        !textureRoiBlanc.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces roi blanc.png") ||
        !textureRoiNoir.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces roi noir.png") ||
        !textureReineBlanche.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces reine blanche.png") ||
        !textureReineNoire.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces reine noire.png") ||
        !textureFouBlanc.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces fou blanc.png") ||
        !textureFouNoir.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces fou noir.png") ||
        !textureCavalierBlanc.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces cavalier blanc.png") ||
        !textureCavalierNoir.loadFromFile("/Users/carraz-billat/Mines/INFO/ProjetEchec/chess pieces/chess pieces cavalier noir.png")) {
        std::cerr << "Erreur de chargement des textures" << std::endl;
        return;
    }

    // Positionnement initial des pièces sur l'échiquier
    sf::Sprite TourBlanc(textureTourBlanc);
    TourBlanc.setPosition(0 * BLOCK_SIZE, 0 * BLOCK_SIZE); // Tour blanche gauche
    sf::Sprite TourNoir(textureTourNoir);
    TourNoir.setPosition(0 * BLOCK_SIZE, 7 * BLOCK_SIZE);  // Tour noire gauche
    sf::Sprite TourBlanc2(textureTourBlanc);
    TourBlanc2.setPosition(7 * BLOCK_SIZE, 0 * BLOCK_SIZE); // Tour blanche droite
    sf::Sprite TourNoir2(textureTourNoir);
    TourNoir2.setPosition(7 * BLOCK_SIZE, 7 * BLOCK_SIZE);  // Tour noire droite

    sf::Sprite CavalierBlanc(textureCavalierBlanc);
    CavalierBlanc.setPosition(1 * BLOCK_SIZE, 0 * BLOCK_SIZE); // Cavalier blanc gauche
    sf::Sprite CavalierNoir(textureCavalierNoir);
    CavalierNoir.setPosition(1 * BLOCK_SIZE, 7 * BLOCK_SIZE);  // Cavalier noir gauche
    sf::Sprite CavalierBlanc2(textureCavalierBlanc);
    CavalierBlanc2.setPosition(6 * BLOCK_SIZE, 0 * BLOCK_SIZE); // Cavalier blanc droit
    sf::Sprite CavalierNoir2(textureCavalierNoir);
    CavalierNoir2.setPosition(6 * BLOCK_SIZE, 7 * BLOCK_SIZE);  // Cavalier noir droit

    sf::Sprite FouBlanc(textureFouBlanc);
    FouBlanc.setPosition(2 * BLOCK_SIZE, 0 * BLOCK_SIZE); // Fou blanc gauche
    sf::Sprite FouNoir(textureFouNoir);
    FouNoir.setPosition(2 * BLOCK_SIZE, 7 * BLOCK_SIZE);  // Fou noir gauche
    sf::Sprite FouBlanc2(textureFouBlanc);
    FouBlanc2.setPosition(5 * BLOCK_SIZE, 0 * BLOCK_SIZE); // Fou blanc droit
    sf::Sprite FouNoir2(textureFouNoir);
    FouNoir2.setPosition(5 * BLOCK_SIZE, 7 * BLOCK_SIZE);  // Fou noir droit

    sf::Sprite ReineBlanche(textureReineBlanche);
    ReineBlanche.setPosition(3 * BLOCK_SIZE, 0 * BLOCK_SIZE); // Reine blanche
    sf::Sprite ReineNoire(textureReineNoire);
    ReineNoire.setPosition(3 * BLOCK_SIZE, 7 * BLOCK_SIZE);  // Reine noire

    sf::Sprite RoiBlanc(textureRoiBlanc);
    RoiBlanc.setPosition(4 * BLOCK_SIZE, 0 * BLOCK_SIZE); // Roi blanc
    sf::Sprite RoiNoir(textureRoiNoir);
    RoiNoir.setPosition(4 * BLOCK_SIZE, 7 * BLOCK_SIZE);  // Roi noir

    std::vector<sf::Sprite> PionsBlancs;
    std::vector<sf::Sprite> PionsNoirs;
    for (int i = 0; i < 8; ++i) {
        sf::Sprite PionBlanc(texturePionBlanc);
        PionBlanc.setPosition(i * BLOCK_SIZE, 1 * BLOCK_SIZE); // Pions blancs
        PionsBlancs.push_back(PionBlanc);
        sf::Sprite PionNoir(texturePionNoir);
        PionNoir.setPosition(i * BLOCK_SIZE, 6 * BLOCK_SIZE); // Pions noirs
        PionsNoirs.push_back(PionNoir);
    }

    sf::Vector2i selectedSquare(-1, -1); // Aucune case sélectionnée
    sf::Vector2i lastmove(-1, -1); // Aucun dernier mouvement
    sf::Sprite* draggedPiece = nullptr; // Pointeur vers la pièce en cours de déplacement
    sf::Vector2f originalPosition; // Position originale de la pièce

    std::vector<sf::Vector2i> accessibleSquares;

    std::vector<sf::Sprite> PiecesBlanches;
    std::vector<sf::Sprite> PiecesNoires;

    // Ajouter toutes les pièces blanches
    PiecesBlanches.push_back(TourBlanc);
    PiecesBlanches.push_back(TourBlanc2);
    PiecesBlanches.push_back(CavalierBlanc);
    PiecesBlanches.push_back(CavalierBlanc2);
    PiecesBlanches.push_back(FouBlanc);
    PiecesBlanches.push_back(FouBlanc2);
    PiecesBlanches.push_back(ReineBlanche);
    PiecesBlanches.push_back(RoiBlanc);
    PiecesBlanches.insert(PiecesBlanches.end(), PionsBlancs.begin(), PionsBlancs.end());

    // Ajouter toutes les pièces noires
    PiecesNoires.push_back(TourNoir);
    PiecesNoires.push_back(TourNoir2);
    PiecesNoires.push_back(CavalierNoir);
    PiecesNoires.push_back(CavalierNoir2);
    PiecesNoires.push_back(FouNoir);
    PiecesNoires.push_back(FouNoir2);
    PiecesNoires.push_back(ReineNoire);
    PiecesNoires.push_back(RoiNoir);
    PiecesNoires.insert(PiecesNoires.end(), PionsNoirs.begin(), PionsNoirs.end());

    // Boucle principale (affichage de la fenêtre et gestion des événements)
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x / BLOCK_SIZE;
                    int y = event.mouseButton.y / BLOCK_SIZE;
                    selectedSquare = sf::Vector2i(x, y);

                    // Vérifier si une pièce est cliquée
                    if (joueurBlanc) {
                        for (auto& piece : PiecesBlanches) {
                            if (piece.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                                draggedPiece = &piece;
                                originalPosition = piece.getPosition();
                                accessibleSquares = calculerCasesAccessibles(draggedPiece, originalPosition, PiecesBlanches, PiecesNoires);
                                break;
                            }
                        }
                    } else {
                        for (auto& piece : PiecesNoires) {
                            if (piece.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                                draggedPiece = &piece;
                                originalPosition = piece.getPosition();
                                accessibleSquares = calculerCasesAccessibles(draggedPiece, originalPosition, PiecesBlanches, PiecesNoires);
                                break;
                            }
                        }
                    }
                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left && draggedPiece) {
                    int x = event.mouseButton.x / BLOCK_SIZE;
                    int y = event.mouseButton.y / BLOCK_SIZE;

                    if (validerDeplacement(sf::Vector2i(x, y), accessibleSquares)) {
                        auto itBlanche = std::find_if(PiecesBlanches.begin(), PiecesBlanches.end(), [&](const sf::Sprite& piece) {
                            return piece.getPosition() == sf::Vector2f(x * BLOCK_SIZE, y * BLOCK_SIZE);
                        });
                        if (itBlanche != PiecesBlanches.end() && &(*itBlanche) != draggedPiece) {
                            PiecesBlanches.erase(itBlanche);
                        }

                        auto itNoire = std::find_if(PiecesNoires.begin(), PiecesNoires.end(), [&](const sf::Sprite& piece) {
                            return piece.getPosition() == sf::Vector2f(x * BLOCK_SIZE, y * BLOCK_SIZE);
                        });
                        if (itNoire != PiecesNoires.end() && &(*itNoire) != draggedPiece) {
                            PiecesNoires.erase(itNoire);
                        }

                        draggedPiece->setPosition(x * BLOCK_SIZE, y * BLOCK_SIZE);
                        if (sf::Vector2i(x, y) != sf::Vector2i(originalPosition.x / BLOCK_SIZE, originalPosition.y / BLOCK_SIZE)) {
                            lastmove = sf::Vector2i(x, y);
                            accessibleSquares.clear();
                            joueurBlanc = !joueurBlanc; // Alterner le joueur
                        }

                        // Vérifier si un roi est capturé
                        if (estRoiCapture(PiecesBlanches, PiecesNoires)) {
                            std::cout << "Le roi a été capturé. Fin de la partie!" << std::endl;
                            window.close();
                        }
                    } else {
                        draggedPiece->setPosition(originalPosition);
                    }

                    draggedPiece = nullptr;
                }
            } else if (event.type == sf::Event::MouseMoved) {
                if (draggedPiece) {
                    draggedPiece->setPosition(event.mouseMove.x - BLOCK_SIZE / 2, event.mouseMove.y - BLOCK_SIZE / 2);
                }
            }
        }
        // Efface la fenêtre avant de dessiner
        window.clear();

        // Dessin des cases de l'échiquier
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (selectedSquare == sf::Vector2i(i, j)) {
                    caseBleue.setPosition(BLOCK_SIZE * i, j * BLOCK_SIZE);
                    window.draw(caseBleue);
                } else if (lastmove == sf::Vector2i(i, j)) {
                    caseRouge.setPosition(BLOCK_SIZE * i, j * BLOCK_SIZE);
                    window.draw(caseRouge);
                } else {
                    if ((i + j) % 2 == 0) {
                        caseBlanc.setPosition(BLOCK_SIZE * i, j * BLOCK_SIZE);
                        window.draw(caseBlanc);
                    } else {
                        caseNoir.setPosition(i * BLOCK_SIZE, j * BLOCK_SIZE);
                        window.draw(caseNoir);
                    }
                }
            }
        }

        // Dessin des cases accessibles en vert
        for (const auto& square : accessibleSquares) {
            caseVerte.setPosition(square.x * BLOCK_SIZE, square.y * BLOCK_SIZE);
            window.draw(caseVerte);
        }

        // Dessin des pièces blanches
        for (const auto& piece : PiecesBlanches) {
            window.draw(piece);
        }

        // Dessin des pièces noires
        for (const auto& piece : PiecesNoires) {
            window.draw(piece);
        }

        // Affiche le contenu dessiné dans la fenêtre
        window.display();
    }
}

int main() {
    échiquier();  // Appelle la fonction pour afficher l'échiquier
    return 0;
}

