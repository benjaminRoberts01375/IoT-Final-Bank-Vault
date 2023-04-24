// Ben Roberts

import AuthenticationServices

final class UserLoginVM {
    public func login(result: Result<ASAuthorization, Error>) {
        switch result {
        case .success(let auth):
            print("Successful login")
            guard let credential = auth.credential as? ASAuthorizationAppleIDCredential else { return }
            UserLoginM.shared.userID = credential.user
            
        case .failure(let error):
            print("Login failed: \(error.localizedDescription)")
        }
    }
}
