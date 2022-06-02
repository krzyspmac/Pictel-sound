//
//  ContentView.swift
//  Pictem-iOS-sample
//
//  Created by krzysp on 31/05/2022.
//

import SwiftUI

struct ContentView: View {
    private var soundPlayer: PictelSoundPlayer = .init(path: Bundle.main.path(forResource: "Rondo_Alla_Turka", ofType: "ogg")!)
    
    var body: some View {
        VStack {
            Text("Pictel-sound-iOS-sample")
                .padding()

            Button("Play sound") {
                soundPlayer.play()
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
