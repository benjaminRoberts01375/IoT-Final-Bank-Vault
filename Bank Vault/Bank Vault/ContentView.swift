// Ben Roberts

import SwiftUI

struct ContentView: View {
    var body: some View {
        NavigationStack {
            List {
                ForEach(Array(UserLoginM.shared.vaults.enumerated()), id: \.element.id) { index, vault in
                    Button(action: {
                        
                    }, label: {
                        Text(vault.name)
                    })
                    .transition(.opacity)
                }
            }
            .edgesIgnoringSafeArea([.bottom, .horizontal])
            .navigationTitle("Your Vaults")
            .toolbar {
                Button(action: {
                    print("Toggle edit mode")
                }, label: {
                    Text("Edit")
                })
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
